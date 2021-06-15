#include <MapFloat.h> //https://github.com/radishlogic/MapFloat

#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
#include <ESP8266WiFi.h>
#endif

//#include <Arduino_LSM6DS3.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


//const char broker[] = "public.cloud.shiftr.io"; // yes this url for SHFTR even with unique instance
const char broker[] =  "val898.cloud.shiftr.io";
int        port     = 1883;
const char topicX[]  = "accX"; //same as js topic
const char topicY[]  = "accY";
const char topicZ[]  = "accZ";
const char accAll[]  = "allAcc";

//char sendMessage[30];

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

//String ID = "111";

String ID = "0";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

//  if (!IMU.begin()) {
//    Serial.println("Failed to initialize IMU!");
//
//    while (1);
//  }

//  Serial.print("Accelerometer sample rate = ");
//  Serial.print(IMU.accelerationSampleRate());
//  Serial.println(" Hz");
//  Serial.println();
//  Serial.println("Acceleration in G's");
//  Serial.println("X\tY\tZ");

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
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  // to avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    float tempX, tempY, tempZ;

      tempX = analogRead(0);
    tempY = analogRead(1);
    tempZ = analogRead(2);

//    if (IMU.accelerationAvailable()) {
//      IMU.readAcceleration(tempX, tempY, tempZ);

      //      Serial.print(tempX);
      //      Serial.print('\t');
      //      Serial.print(tempY);
      //      Serial.print('\t');
      //      Serial.println(tempZ);
//    }
    int x = tempX * 1000;
    int y = tempY * 1000;
    int z = tempZ * 1000;

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);

    int diffX = abs (oldX - x);
    int diffY = abs (oldY - y);
    int diffZ = abs (oldZ - z);

    if (diffX < 30) x = oldX;
    if (diffY < 30) y = oldY;
    if (diffZ < 30) z = oldZ;
    //
    //     Serial.print(x);
    //      Serial.print('\t');
    //      Serial.print(y);
    //      Serial.print('\t');
    //      Serial.print(z);
    //      Serial.print('\t');
    //
    //       Serial.print(diffX);
    //      Serial.print('\t');
    //      Serial.print(diffY);
    //      Serial.print('\t');
    //      Serial.println(diffZ);

    //sprintf(sendMessage, "%.9f,%.9f,%.9f", x, y, z);
    String sendMessage = String(ID) + "," + String(x) + "," + String(y) + "," + String(z);
    //Serial.println(sendMessage);
    mqttClient.beginMessage(accAll);
    mqttClient.print(sendMessage);
    mqttClient.endMessage();


    oldX = x;
    oldY = y;
    oldZ = z;

  }
}
