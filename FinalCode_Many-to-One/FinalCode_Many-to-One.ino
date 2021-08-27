//------------------Many-to-One Final code for MKR1000------------------
//------------by Kum San Vanessa Amy Dang-Lam----------


#include <MapFloat.h> //https://github.com/radishlogic/MapFloat

#include <ArduinoMqttClient.h>

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <SPI.h>
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#endif

//--------Accelerometer Library--------
#include <Arduino_LSM6DS3.h>

//--------Neopixel Library & Setting--------
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#endif

#define PIN        9 //Neopixel pin
#define NUMPIXELS 7 //Neopixel LED size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//--------WiFi Authentification--------
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "vdl898.cloud.shiftr.io";
int        port     = 1883;
const char accAll[]  = "allAcc";
const char topicArdAllAcc[] = "ardAllAcc";


//-------Send-------
const long interval = 100;
unsigned long previousMillis = 0;

float floatX, floatY, floatZ;
int mappedX, mappedY, mappedZ;

int oldX, oldY, oldZ;

char ID[13];
byte mac[6];                     // the MAC address of your Wifi shield
char ID_Char[13];


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

int accX, accY, accZ;
char ID_Receive[13];

boolean needParse = false;
// see the message parse tab for this
char cleanMessage[mqttBufferSize];
char *bufferSplit[8]; // array of 8 pointers to chars -- ie message max 8 tokens

//--------DEBUG Function--------
//#define DEBUG //uncomment this to see all the Serial.print, Serial.println, keep commented if you don't need to see the prints
#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#define DEBUG_PRINT_FLOAT(x,y) Serial.println(x,y);
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

//-------Neopixel Function------
char id;
int neoX, neoY, neoZ;

//-------------------------------SETUP-------------------------------
void setup() {
  //--------Initialize serial--------
  Serial.begin(115200);
  delay(2000);
  Serial.println("--------Start setup!--------");
  Serial.println();
  delay(1000);

  //--------Connect Wifi network--------
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print("failed");
    delay(3000);
  }
  Serial.println("You're connected to the network");
  Serial.println();
  
  //--------MacAddress--------
  WiFi.macAddress(mac);
  IDs();
  Serial.println();

  //--------Accelerometer connect--------
  if (!IMU.begin()) { //Accelerometer
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU initialized!");
  Serial.println();

  //--------Shiftr/MQTT--------
  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  mqttClient.setId(ID);

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
  mqttClient.onMessage(mqttMessageEvent);

  // subscribe to a topic
  mqttClient.subscribe(topicArdAllAcc);

  //--------Neopixel begins--------
  pixels.begin();
  pixels.setBrightness(10);

  Serial.println("--------End setup!--------");
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

    int diffX = abs (oldX - x);
    int diffY = abs (oldY - y);
    int diffZ = abs (oldZ - z);

    if (diffX < 30) x = oldX;
    if (diffY < 30) y = oldY;
    if (diffZ < 30) z = oldZ;

    String sendMessage = String(ID) + "," + String(x) + "," + String(y) + "," + String(z);
    mqttClient.beginMessage(accAll);
    mqttClient.print(sendMessage);
    mqttClient.endMessage();

    oldX = x;
    oldY = y;
    oldZ = z;

    DEBUG_PRINTLN(currentMillis);
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

  String STRING_of_Topic = mqttClient.messageTopic();

  // if you want to use char arrays -- but the lib returns string
  STRING_of_Topic.toCharArray(mqttIncomingTopic, maxTopicLength);

  //sprintf(mqttIncomingTopic, "%s", mqttClient.messageTopic());
  // we received a message, print out the topic and contents
  DEBUG_PRINT("Received a message with topic: '");
  DEBUG_PRINT(STRING_of_Topic);
  DEBUG_PRINT("', length ");
  DEBUG_PRINT(messageSize);
  DEBUG_PRINTLN(" bytes.");
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
  DEBUG_PRINT("Message Status ");
  DEBUG_PRINTLN(messageStatus);

  if ( 1 == messageStatus ) {
    // got a complete message with start and end
    // start end  have been removed ( cleaned)
    // message now resides in char cleanMessage[]

    // next, match message to topic type

    // switch topics basic on incoming message


    // use this to match ardAllAcc
    if (strcmp(mqttIncomingTopic, "ardAllAcc") == 0) {
      if (needParse) {
        tokenStatus = parseMessage();
        //FWIW i am mixing global and locals here .... clean up as needed
        if (1 == tokenStatus) {

          strcpy (ID_Receive, bufferSplit[0]); //str1 copies to str2
          accX         = atoi (bufferSplit[1]);
          accY         = atoi (bufferSplit[2]);
          accZ         = atoi (bufferSplit[3]);

          //-------set Neopixels according to ID---------
          // check message ID against wristband ID if same change LEDs
          if (strcmp(ID_Receive, ID) == 0) {
            setLED(accX, accY, accZ);
          }
        } else { // incomplete message
          DEBUG_PRINTLN ("in ardAllAcc parse failed");
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

  int status = 0;
  // should not get here unless connected to wifi
  if ( WiFi.status() == WL_CONNECTED) {
    WiFi.macAddress(mac);
    Serial.print("My Id is: ");
    sprintf(ID, "%02X%02X%02X%02X%02X%02X%c", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0], '\0');
    Serial.println(ID);
    status = 1;
  }
}

//--------Neopixel Setup--------
void setLED(int accX, int accY, int accZ) {
  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(accX, accY, accZ));
    pixels.show();
  }
}
