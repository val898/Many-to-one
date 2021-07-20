#include <MapFloat.h> //https://github.com/radishlogic/MapFloat

#include <ArduinoMqttClient.h>

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <SPI.h>
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
#include <ESP8266WiFi.h>
#endif

//--------Accelerometer Library--------
#include <Arduino_LSM6DS3.h>

//--------Neopixel Library & Setting--------
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        6 //Neopixel pin
#define NUMPIXELS 7 //Neopixel LED size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//--------Wifi Authentification--------
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
//Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char broker[] = "public.cloud.shiftr.io"; // yes this url even when unique instance
int        port     = 1883;
const char accAll[]  = "allAcc";
const char topicArdAllAcc[] = "ardAllAcc";


//-------Send-------
const long interval = 100;
unsigned long previousMillis = 0;


float floatX;
int mappedX;
float floatY;
int mappedY;
float floatZ;
int mappedZ;

int oldX;
int oldY;
int oldZ;

//String ID = "1";
char ID[13];
byte mac[6];                     // the MAC address of your Wifi shield
char ID_Char[13];
String ID_String;
String temp;


//-------Receive------
// input buffer
// will capture string from MQTT and parse then act
const int mqttBufferSize  = 128;
int  lastMessageSize = mqttBufferSize;
char mqttIncomingBuffer[mqttBufferSize]; // 128 char buffer -- keep message shorter than this
//--array of characters


// incoming ? clarify
const int maxTopicLength = 16;
char mqttIncomingTopic[maxTopicLength];
char mqttSendTopic[maxTopicLength];
//--pull out characters of the array


char START_MARKER = '*';
char END_MARKER = '#';
char DELIMITER = ',';
//--define the markers

int ID_Receive, accX, accY, accZ;

boolean needParse = false;
// see the message parse tab for this
char cleanMessage[mqttBufferSize];
char *bufferSplit[8]; // array of 8 pointers to chars -- ie message max 8 tokens

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#define DEBUG_PRINT_FLOAT(x,y) Serial.println(x,y);
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

void setup() {
  //--------Initialize serial--------
  Serial.begin(9600);

  //--------Connect Wifi network--------
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print("failed");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  //--------MacAddress--------
  WiFi.macAddress(mac);
  IDs();
  Serial.println();

  //--------Accelerometer connect--------
  if (!IMU.begin()) { //IoT builtin board
    //if (!lsm6ds33.begin_I2C()) { //Breakout board
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //  Serial.print("Accelerometer sample rate = ");
  //  Serial.print(IMU.accelerationSampleRate());
  //  Serial.println(" Hz");
  //  Serial.println();
  //  Serial.println("Acceleration in G's");
  //  Serial.println("X\tY\tZ");

  //--------Shiftr/MQTT--------
  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
//  mqttClient.setId(SHIFTR_ID);
  mqttClient.setId(ID_String);

  // You can provide a username and password for authentication
  mqttClient.setUsernamePassword(SHFTR_INSTANCE, SHFTR_TOKEN); // instance name, token secret -- if raw in quotes

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  //  mqttClient.onMessage(onMqttMessage);
  mqttClient.onMessage(mqttMessageEvent);

  // subscribe to a topic
  mqttClient.subscribe(topicArdAllAcc);

  //--------Neopixel--------
  pixels.begin();
  pixels.setBrightness(10);
}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alives which avoids being disconnected by the broker
  mqttClient.poll();

  //--------Send Accelerometer Data--------
  // to avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    float tempX, tempY, tempZ;

    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(tempX, tempY, tempZ);
    }

    int x = tempX * 1000;
    int y = tempY * 1000;
    int z = tempZ * 1000;

    //    Serial.print(x);
    //    Serial.print('\t');
    //    Serial.print(y);
    //    Serial.print('\t');
    //    Serial.println(z);

    int diffX = abs (oldX - x);
    int diffY = abs (oldY - y);
    int diffZ = abs (oldZ - z);

    if (diffX < 30) x = oldX;
    if (diffY < 30) y = oldY;
    if (diffZ < 30) z = oldZ;

    String sendMessage = String(ID_String) + "," + String(x) + "," + String(y) + "," + String(z);
    //Serial.println(sendMessage);
    mqttClient.beginMessage(accAll);
    mqttClient.print(sendMessage);
    mqttClient.endMessage();

    oldX = x;
    oldY = y;
    oldZ = z;
  }
}

//--------Receive Node.js Message Function--------
// receive messages here
void mqttMessageEvent(int messageSize) {

  // error check and keep message inside buffer
  if (messageSize > mqttBufferSize) messageSize = mqttBufferSize;
  if (lastMessageSize > mqttBufferSize) lastMessageSize = mqttBufferSize;
  //--resize the buffer by chopping off hte end

  clearCharArray(mqttIncomingBuffer, lastMessageSize);
  //mqttIncomingBuffer[0] = '\0';

  String STRING_of_Topic = mqttClient.messageTopic();

  // if you want to use char arrays -- but the lib returns string
  STRING_of_Topic.toCharArray(mqttIncomingTopic, maxTopicLength);

  //sprintf(mqttIncomingTopic, "%s", mqttClient.messageTopic());
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic: '");
  Serial.print(STRING_of_Topic);
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes.");
  lastMessageSize = messageSize;

  static int messageStatus;
  static int tokenStatus;
  //source with mods : https://forum.arduino.cc/index.php?topic=396450.0


  // read the buffer -- load up the char array
  char c;
  byte count = 0;
  while (mqttClient.available()) {
    // what if you have not finished with last message ?
    c = mqttClient.read();
    mqttIncomingBuffer[count] = c;
    count++;
  }
  mqttIncomingBuffer[messageSize] = '\0'; // put null terminator at end of string

  // check for message with start / end bytes
  messageStatus = checkMessage(mqttIncomingBuffer, messageSize);

  if ( 1 == messageStatus ) {
    //    Serial.println("got one");
    //    Serial.println(mqttIncomingTopic);
    // got a complete message with start and end
    // start end  have been removed ( cleaned)
    // message now resides in char cleanMessage[]

    // next, match message to topic type

    // switch topics basic on incoming message


    // use this to match ardAllAcc
    if (strcmp(mqttIncomingTopic, "ardAllAcc") == 0) {
      //      Serial.println("in strcmp got /ardAllAcc");
      if (needParse) {
        tokenStatus = parseMessage();
        //FWIW i am mixing global and locals here .... clean up as needed
        if (1 == tokenStatus) {
          //          Serial.print("in message ardAllAcc --> bufferSplit[1] :: \t");
          //          Serial.println(bufferSplit[0]);
          //          Serial.print('\t');
          //          Serial.print(bufferSplit[1]);
          //          Serial.print('\t');
          //          Serial.print(bufferSplit[2]);
          //          Serial.print('\t');
          //          Serial.println(bufferSplit[3]);

          ID_Receive   = atoi (bufferSplit[0]);
          accX         = atoi (bufferSplit[1]);
          accY         = atoi (bufferSplit[2]);
          accZ         = atoi (bufferSplit[3]);

//          setLED((int)ID, accX, accY, accZ);

if (ID_Receive = (int)ID) {
    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(accX, accY, accZ));
      pixels.show();
    }
  }
        } else {
          Serial.println ("in ardAllAcc parse failed");
        }
        needParse = false;
      }
    }// end allAcc

  } // end messageStatus

} // end messageEvent


//--------Get MacAddress function--------
int getMyId() {
  int status = 0;
  // should not get here unless connected to wifi
  if ( WiFi.status() == WL_CONNECTED) {
    WiFi.macAddress(mac);
    sprintf(ID, "%02X%02X%02X%02X%02X%02X%c", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0], '\0');
    status = 1;
  }
  return status;
}

void IDs() {
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

  for (int j = 5; j >= 0; j--) {
    temp = String(mac[j], DEC);
    ID_String += temp; // mac[5]+mac[4]+mac[3]+mac[2]+mac[1]+mac[0];
  }
  Serial.print("My ID in String: ");
  Serial.println(ID_String);
}

//--------Neopixel Setup--------
//void setLED(int id, int neoX, int neoY, int neoZ) {
//  IDs();
//  if (ID_Receive = (int)ID) {
//    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
//      pixels.setPixelColor(i, pixels.Color(neoX, neoY, neoZ));
//      pixels.show();
//    }
//  }
//}
