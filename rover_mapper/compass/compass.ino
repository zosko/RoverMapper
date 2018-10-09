#include <Wire.h>
#include <SoftwareSerial.h>
#include "HMC5883L_Simple.h"
//#include "HMC5983.h"
//#include "Adafruit_Sensor.h"
//#include "Adafruit_HMC5883_U.h"

HMC5883L_Simple Compass;
//HMC5983 compass;
//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
SoftwareSerial communication(2, 3);

float prevHeader = 0;

void sendIntSerial(int x) {
  uint8_t LSB = x;
  uint8_t MSB = x >> 8;
  communication.write(MSB);
  communication.write(LSB);
}
void setup() {
  Wire.begin();
  Serial.begin(9600);
  communication.begin(9600);

  //  compass.begin();

    Compass.SetSamplingMode(0x00);
    Compass.SetScale(COMPASS_SCALE_250);
    Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);
    Compass.SetDeclination(4, 34, 'E');

//  if (!mag.begin()) {
//    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
//    while (1);
//  }
}

void loop() {
  //  float c = -999;
  //  c = compass.read(); // heading in 0-360º format
  //  if (c == -999) {
  //    Serial.println(prevHeader);
  //    sendIntSerial(prevHeader);
  //  }
  //  else {
  //    prevHeader = c;
  //    Serial.println(c);
  //    sendIntSerial(c);
  //  }
  //  delay(100);


  float c = Compass.GetHeadingDegrees();
  sendIntSerial(c);
  Serial.println(c);
  delay(100);



  //  sensors_event_t event;
  //  mag.getEvent(&event);
  //  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  //  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  //  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  "); Serial.println("uT");
  //
  //  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  //  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  //  float heading = atan2(event.magnetic.y, event.magnetic.x);
  //
  //  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  //  // Find yours here: http://www.magnetic-declination.com/
  //  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  //  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  ////  float declinationAngle = 0.22;
  ////  heading += declinationAngle;
  //
  //  // Correct for when signs are reversed.
  //  if (heading < 0)
  //    heading += 2 * PI;
  //
  //  // Check for wrap due to addition of declination.
  //  if (heading > 2 * PI)
  //    heading -= 2 * PI;
  //
  //  // Convert radians to degrees for readability.
  //  float headingDegrees = heading * 180 / M_PI;
  //  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  //  delay(500);
}
