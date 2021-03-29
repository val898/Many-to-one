// How to control the RGB Led and Power Led of the Nano 33 BLE boards.
// The three LEDs in the RGB LED, however, are active-low.

#define RED 22
#define BLUE 24
#define GREEN 23

/*
  APDS9960 - Gesture Sensor

  This example reads gesture data from the on-board APDS9960 sensor of the
  Nano 33 BLE Sense and prints any detected gestures to the Serial Monitor.

  Gesture directions are as follows:
  - UP:    from USB connector towards antenna
  - DOWN:  from antenna towards USB connector
  - LEFT:  from analog pins side towards digital pins side
  - RIGHT: from digital pins side towards analog pins side
*/

#include <Arduino_APDS9960.h>

//BLE peripheral
#include <ArduinoBLE.h>

BLEService gestureService("52c0fb16-8ced-11eb-8dcd-0242ac130003"); // BLE gesture Service -Random generator https://www.uuidgenerator.net/

// BLE Gesture sensor Characteristic - custom 128-bit UUID, read by central
BLEByteCharacteristic gesCharacteristic("52c0fb17-8ced-11eb-8dcd-0242ac130003", BLERead);
//BLEByteCharacteristic RGBCharacteristic("52c0fb18-8ced-11eb-8dcd-0242ac130003", BLENotify);

//---------------------------------------------------------
void setup() {
  // put your setup code here, to run once:

  // intitialize the digital Pin as an output
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);

  Serial.begin(9600);
  while (!Serial);


// BLE conenction
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("GestureRGB");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(gestureService);

  // add the characteristics to the service
  gestureService.addCharacteristic(gesCharacteristic);
//  gestureService.addCharacteristic(RGBCharacteristic);

  // add the service
  BLE.addService(gestureService);

  gesCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");

  //  Gesture sensor
  // for setGestureSensitivity(..) a value between 1 and 100 is required.
  // Higher values makes the gesture recognition more sensible but less accurate
  // (a wrong gesture may be detected). Lower values makes the gesture recognition
  // more accurate but less sensible (some gestures may be missed).
  // Default is 80
  //APDS.setGestureSensitivity(80);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }
}


//---------------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:

  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    Serial.println("Detecting gestures ...");

//Sensor code
    // while the central is still connected to peripheral:
    while (central.connected()) {
      if (APDS.gestureAvailable()) {
      }
      // a gesture was detected, read and print to serial monitor
      int gesture = APDS.readGesture();

      switch (gesture) {
        case GESTURE_UP:
          Serial.print(gesture + 1);
          Serial.print(" -- Detected UP gesture");
          digitalWrite(RED, HIGH);
          digitalWrite(BLUE, LOW);
          digitalWrite(GREEN, LOW);
          Serial.println(" ---> CYAN");
          gesCharacteristic.writeValue(gesture + 1);
          break;

        case GESTURE_DOWN:
          Serial.print(gesture + 1);
          Serial.print(" -- Detected DOWN gesture");
          digitalWrite(BLUE, HIGH);
          digitalWrite(RED, LOW);
          digitalWrite(GREEN, LOW);
          Serial.println(" ---> YELLOW");
          gesCharacteristic.writeValue(gesture + 1);
          break;

        case GESTURE_LEFT:
          Serial.print(gesture + 1);
          Serial.print(" -- Detected LEFT gesture");
          digitalWrite(GREEN, HIGH);
          digitalWrite(BLUE, LOW);
          digitalWrite(RED, LOW);
          Serial.println(" ---> PINK");
          gesCharacteristic.writeValue(gesture + 1);
          break;

        case GESTURE_RIGHT:
          Serial.print(gesture + 1);
          Serial.print(" -- Detected RIGHT gesture");
          digitalWrite(RED, HIGH);
          digitalWrite(BLUE, HIGH);
          digitalWrite(GREEN, LOW);
          Serial.println(" ---> GREEN");
          gesCharacteristic.writeValue(gesture + 1);
          break;
      }
    }
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
