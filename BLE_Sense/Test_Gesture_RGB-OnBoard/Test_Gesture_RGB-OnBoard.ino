// How to control the RGB Led and Power Led of the Nano 33 BLE boards.  
// The three LEDs in the RGB LED, however, are active-low.

 #define RED 22     
 #define BLUE 24     
 #define GREEN 23
 #define LED_PWR 25

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
 
void setup() {
  // put your setup code here, to run once:
  
  // intitialize the digital Pin as an output
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);

  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }

  Serial.println("Detecting gestures ...");
}

void loop() {
  // put your main code here, to run repeatedly:

 if (APDS.gestureAvailable()) {
    // a gesture was detected, read and print to serial monitor
    int gesture = APDS.readGesture();

    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        digitalWrite(RED, HIGH);
        digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, LOW);
        break;

      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        digitalWrite(BLUE, HIGH);
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);
        break;

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        digitalWrite(GREEN, HIGH);
        digitalWrite(BLUE, LOW);
        digitalWrite(RED, LOW);
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        digitalWrite(RED, HIGH);
        digitalWrite(BLUE, HIGH);
        digitalWrite(GREEN, LOW);
        break;
    }
  }
}
