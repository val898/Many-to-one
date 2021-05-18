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

const char broker[] = "public.cloud.shiftr.io"; // yes this url even when unique instance
int        port     = 1883;
const char topic[]  = "valacc";

int led = 6; //LED pin
int LED_STATE = 0;
int fadeAmount = 5; //LED fade points


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();

   pinMode(led, OUTPUT);
}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alives which avoids being disconnected by the broker
  mqttClient.poll();
}

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.println("'");
  Serial.print("length: ");
  Serial.println(messageSize);
//  Serial.print("MQTT read: ");
//  Serial.println(mqttClient.read());

  // use the Stream interface to print the contents
String temp = "";
  while (mqttClient.available()) {
    char c = mqttClient.read();
    temp += c;
    }
    Serial.println(temp);
    
//    LED_STATE = mqttClient.read();
LED_STATE = temp.toInt();
Serial.print("LED_STATE ");
Serial.println(LED_STATE);

analogWrite(led, LED_STATE);

//LED_STATE = LED_STATE + fadeAmount;
if (LED_STATE <= 0 || LED_STATE >= 255){
  fadeAmount = -fadeAmount;
}
  Serial.println();
}
