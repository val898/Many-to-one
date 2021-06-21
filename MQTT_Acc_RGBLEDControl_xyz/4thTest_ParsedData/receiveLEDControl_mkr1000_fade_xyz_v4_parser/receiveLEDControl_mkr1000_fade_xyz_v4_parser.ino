/*
  ArduinoMqttClient - WiFi Simple Receive Callback
  This example connects to a MQTT broker and subscribes to a single topic.
  When a message is received it prints the message to the serial monitor,
  it uses the callback functionality of the library.
  The circuit:
  - Arduino MKR 1000, MKR 1010, Uno WiFi Rev.2 board or nano iot 33
  This example code is in the public domain.
*/

#include <ArduinoMqttClient.h>

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
#include <ESP8266WiFi.h>
#endif

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

//const char broker[] = "public.cloud.shiftr.io"; // yes this url even when unique instance
const char broker[] = "val898.cloud.shiftr.io";
int        port     = 1883;
//const char topicX[]  = "accX";
//const char topicY[]  = "accY";
//const char topicZ[]  = "accZ";
//const char topicID[] = "ID";
const char topicArdAllAcc[] = "ardAllAcc";


int LED_STATE = 0;
//int LED_STATE1 = 0;

const int redPin = 6;
const int greenPin = 7;
const int bluePin = 8;

const int redPin1 = 11;
const int greenPin1 = 10;
const int bluePin1 = 9;

int redValue, greenValue, blueValue;
String IDValue;
int redValue1, greenValue1, blueValue1;

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

int ID, accX, accY, accZ;

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
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for native USB port only
  //  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  mqttClient.setId(SHIFTR_ID); // any name works here -- be smart about it

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

  //  Serial.print("Subscribing to topic: ");
  //  Serial.print(topicX);
  //  Serial.print('\t');
  //  Serial.print(topicY);
  //  Serial.print('\t');
  //  Serial.println(topicZ);
  //  Serial.println();

  // subscribe to a topic
  //  mqttClient.subscribe(topicX);
  //  mqttClient.subscribe(topicY);
  //  mqttClient.subscribe(topicZ);
  //  mqttClient.subscribe(topicID);
  mqttClient.subscribe(topicArdAllAcc);

  //  Serial.print("Waiting for messages on topic: ");
  //  Serial.print(topicX);
  //  Serial.print('\t');
  //  Serial.print(topicY);
  //  Serial.print('\t');
  //  Serial.print(topicZ);
  //  Serial.print('\t');
  //  Serial.println(topicID);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);
}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alives which avoids being disconnected by the broker
  mqttClient.poll();
}

//void onMqttMessage(int messageSize) {
//  // we received a message, print out the topic and contents
//  String String_topic = mqttClient.messageTopic();
//  Serial.print("Received a message with topic '");
//  Serial.print(String_topic);
//  Serial.println("'");
//  Serial.print("length: ");
//  Serial.println(messageSize);
//
//  // use the Stream interface to print the contents
//  String temp = "";
//  while (mqttClient.available()) {
//    char c = mqttClient.read();
//    temp += c;
//  }
//  Serial.println(temp);
//
//  LED_STATE = temp.toInt();
//  LED_STATE1 = temp.toInt();
//
//  IDValue = "ID";
//
//
//  if (IDValue = "0") {
//    if (String_topic == "accX") {
//      redValue = LED_STATE;
//      analogWrite(redPin, redValue);
//    }
//
//    if (String_topic == "accY") {
//      greenValue = LED_STATE;
//      analogWrite(greenPin, greenValue);
//    }
//
//    if (String_topic == "accZ") {
//      blueValue = LED_STATE;
//      analogWrite(bluePin, blueValue);
//    }
//  }
//
//  if (IDValue = "1") {
//    if (String_topic == "accX") {
//      redValue1 = LED_STATE1;
//      analogWrite(redPin1, redValue1);
//    }
//
//    if (String_topic == "accY") {
//      greenValue1 = LED_STATE1;
//      analogWrite(greenPin1, greenValue1);
//    }
//
//    if (String_topic == "accZ") {
//      blueValue1 = LED_STATE1;
//      analogWrite(bluePin1, blueValue1);
//    }
//  }

//}

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

  // use the Stream interface to get the contents and put in char array
  //  Serial.println("before we get the new data");
  //  printBuffer(mqttIncomingBuffer, messageSize); // debug


  //get the message
  //
  // boolean messageComplete = false; // track if whole massage has arrived
  // static boolean recvInProgress = false;
  // static byte ndx = 0;
  // char startMarker = '*';
  // char endMarker = '#';
  // char c;
  // int static messageStatus = 4;

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
    Serial.println("got one");
    Serial.println(mqttIncomingTopic);
    // got a complete message with start and end
    // start end  have been removed ( cleaned)
    // message now resides in char cleanMessage[]

    // next, match message to topic type

    // switch topics basic on incoming message


    // use this to match ardAllAcc
    if (strcmp(mqttIncomingTopic, "ardAllAcc") == 0) {
      Serial.println("in strcmp got /ardAllAcc");
      if (needParse) {
        tokenStatus = parseMessage();
        //FWIW i am mixing global and locals here .... clean up as needed
        if (1 == tokenStatus) {
          Serial.print("in message ardAllAcc --> bufferSplit[1] :: \t");
          Serial.println(bufferSplit[0]);
          Serial.println(bufferSplit[1]);
          Serial.println(bufferSplit[2]);
          Serial.println(bufferSplit[3]);

          ID   = atoi (bufferSplit[0]);
          accX = atoi (bufferSplit[1]);
          accY = atoi (bufferSplit[2]);
          accZ = atoi (bufferSplit[3]);

          setLED(ID, accX, accY, accZ);


        } else {
          Serial.println ("in ardAllAcc parse failed");
        }
        needParse = false;
      }
    }// end allAcc


    // insert new messages here

    //Copy and paste lines 112 - 137 if there need new token

  } // end messageStatus

} // end messageEvent

void setLED(int id, int red, int green, int blue) {
  if (id == 0) {
    analogWrite (redPin, red);
    analogWrite (greenPin, green);
    analogWrite (bluePin, blue);
  }
  if (id == 1) {
    analogWrite (redPin1, red);
    analogWrite (greenPin1, green);
    analogWrite (bluePin1, blue);
  }
}
