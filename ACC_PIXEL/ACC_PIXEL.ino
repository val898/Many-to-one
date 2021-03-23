#include <Adafruit_NeoPixel.h>

#define PIN 6
#define LED_COUNT 2

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

int x, y, z;

void setup() {
  // put your setup code here, to run once:
  leds.begin();
  leds.show();
  Serial.begin(9600);
}

int counter = 0;

void loop() {
  // put your main code here, to run repeatedly:
  
  x = analogRead(0);
  y = analogRead(1);
  z = analogRead(2);
  Serial.print("Accelerations x, y, z: ");
  Serial.print(x, DEC);
  Serial.print(" ");
  Serial.print(y, DEC);
  Serial.print(" ");
  Serial.println(z, DEC);
  counter++;

  if(x > 500) {
//      rainbow(1);
      leds.setPixelColor (0, 255, 255, 0); //YELLOW
      leds.show();
      delay(200);
      Serial.print("Accelerations x, y, z: ");
      Serial.print(x, DEC);
      Serial.print(" ");
      Serial.print(y, DEC);
      Serial.print(" ");
      Serial.print(z, DEC);
      Serial.print(" ==> XCOLOR: ");
//      Serial.println("Rainbow");
      Serial.print("YELLOW");
      delay(200);
      delay(1000);
      
      if(y > 500) {
          leds.setPixelColor (1, 0, 255, 0); //GREEN
          leds.show();
          delay(200);
          Serial.print(" ==> YCOLOR: ");
          Serial.println("GREEN");
          delay(200);
          }
        
       else {
          leds.setPixelColor (1, 0, 0, 255); //BLUE
          leds.show();
          delay(200);
          Serial.print(" ==> YCOLOR: ");
          Serial.println("BLUE");
          delay(200);
          delay(1000);
        }
        delay(100);
    }
    
   else {
      leds.setPixelColor (0, 255, 0, 0); //RED
      leds.show();
      delay(200);
      Serial.print("Accelerations x, y, z: ");
      Serial.print(x, DEC);
      Serial.print(" ");
      Serial.print(y, DEC);
      Serial.print(" ");
      Serial.print(z, DEC);
      Serial.print(" ==> XCOLOR: ");
      Serial.print("RED");
      delay(200);
      delay(1000);

      if(y > 500) {
          leds.setPixelColor (1, 0, 255, 0); //GREEN
          leds.show();
          delay(200);
          Serial.print(" ==> YCOLOR: ");
          Serial.println("GREEN");
          delay(200);
          }
        
       else {
          leds.setPixelColor (1, 0, 0, 255); //BLUE
          leds.show();
          delay(200);
          Serial.print(" ==> YCOLOR: ");
          Serial.println("BLUE");
          delay(200);
          delay(1000);
          }
        delay(100);
    }
}
//    if(y > 400) {
//    for (int i=0; i<LED_COUNT*5; i++) {
//      leds.setPixelColor (2, 0, 255, 0); //GREEN
//      delay(200);
//      Serial.print("Accelerations x, y, z: ");
//      Serial.print(x, DEC);
//      Serial.print(" ");
//      Serial.print(y, DEC);
//      Serial.print(" ");
//      Serial.print(z, DEC);
//      Serial.print(" ==> COLOR: ");
//      Serial.println("GREEN");
//      delay(200);
//      }
//    }
//   if(y <= 400) {
//   for (int i=0; i<LED_COUNT*5; i++) {
//      leds.setPixelColor (2, 0, 0, 255); //BLUE
//      delay(200);
//      Serial.print("Accelerations x, y, z: ");
//      Serial.print(x, DEC);
//      Serial.print(" ");
//      Serial.print(y, DEC);
//      Serial.print(" ");
//      Serial.print(z, DEC);
//      Serial.print(" ==> COLOR: ");
//      Serial.println("BLUE");
//      delay(200);
//      delay(1000);
//      }
//    }
//    delay(100);
//}

//
//// Prints a rainbow on the ENTIRE LED strip.
////  The rainbow begins at a specified position. 
//// ROY G BIV!
//void rainbow(byte startPosition) 
//{
//  // Need to scale our rainbow. We want a variety of colors, even if there
//  // are just 10 or so pixels.
//  int rainbowScale = 192 / LED_COUNT;
//  
//  // Next we setup each pixel with the right color
//  for (int i=0; i<LED_COUNT; i++)
//  {
//    // There are 192 total colors we can get out of the rainbowOrder function.
//    // It'll return a color between red->orange->green->...->violet for 0-191.
//    leds.setPixelColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
//  }
//  // Finally, actually turn the LEDs on:
//  leds.show();
//}
//
//// Input a value 0 to 191 to get a color value.
//// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
////  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
//uint32_t rainbowOrder(byte position) 
//{
//  // 6 total zones of color change:
//  if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
//  {
//    return leds.Color(0xFF, position * 8, 0);
//  }
//  else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
//  {
//    position -= 31;
//    return leds.Color(0xFF - position * 8, 0xFF, 0);
//  }
//  else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
//  {
//    position -= 63;
//    return leds.Color(0, 0xFF, position * 8);
//  }
//  else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
//  {
//    position -= 95;
//    return leds.Color(0, 0xFF - position * 8, 0xFF);
//  }
//  else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
//  {
//    position -= 127;
//    return leds.Color(position * 8, 0, 0xFF);
//  }
//  else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
//  {
//    position -= 159;
//    return leds.Color(0xFF, 0x00, 0xFF - position * 8);
//  }
//}
