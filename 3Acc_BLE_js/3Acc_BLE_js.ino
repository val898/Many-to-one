/*
  Arduino LSM9DS1 - Simple Accelerometer

  This example reads the acceleration values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.
*/
#include <Arduino_LSM9DS1.h>

//BLE peripheral
#include <ArduinoBLE.h>

BLEService acceleroService("52c0fb16-8ced-11eb-8dcd-0242ac130003"); // BLE gesture Service -Random generator https://www.uuidgenerator.net/

// BLE Accelerometer sensor Characteristic - custom 128-bit UUID, read by central
BLEByteCharacteristic accCharacteristic("52c0fb17-8ced-11eb-8dcd-0242ac130003", BLERead || BLENotify);
BLEByteCharacteristic gyroCharacteristic("52c0fb18-8ced-11eb-8dcd-0242ac130003", BLERead || BLENotify);

// Include Madgwick Library to Get Orientation
#include <MadgwickAHRS.h>

// initialize Madgwick filter
Madgwick filter;

// sensor's sample rate is fixed at 104 Hz:
const float sensorRate = 104.00;

// values for orientation:
float roll = 0.0;
float pitch = 0.0;
float heading = 0.0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // BLE conenction
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (true);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Accelerometer");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(acceleroService);

  // add the characteristics to the service
  acceleroService.addCharacteristic(accCharacteristic);
  acceleroService.addCharacteristic(gyroCharacteristic);

  // add the service
  BLE.addService(acceleroService);

  accCharacteristic.writeValue(0);
  gyroCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (true);
  }

  // start the filter to run at the sample rate:
    filter.begin(sensorRate);
}


void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // Accelerometer sensor
    // while the central is still connected to peripheral:
    while (central.connected()) {

      // values for acceleration and rotation:
      float xAcc, yAcc, zAcc;
      float xGyro, yGyro, zGyro;

      // values for orientation:
      float roll, pitch, heading;

      if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
        IMU.readAcceleration(xAcc, yAcc, zAcc);
        IMU.readGyroscope(xGyro, yGyro, zGyro);

        Serial.print(xAcc);
        Serial.print('\t');
        Serial.print(yAcc);
        Serial.print('\t');
        Serial.print(zAcc);
        Serial.print('\t');
        Serial.print(xGyro);
        Serial.print('\t');
        Serial.print(yGyro);
        Serial.print('\t');
        Serial.println(zGyro);

     // update the filter, which computes orientation:
                filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);
        
                // print the heading, pitch and roll
                roll = filter.getRoll();
                pitch = filter.getPitch();
                heading = filter.getYaw();
        
                // if you get a byte in the serial port,
                // send the latest heading, pitch, and roll:
                if (Serial.available()) {
                  char input = Serial.read();
                  Serial.print(heading);
                  Serial.print(",");
                  Serial.print(pitch);
                  Serial.print(",");
                  Serial.println(roll);
                }
      }

    }
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
