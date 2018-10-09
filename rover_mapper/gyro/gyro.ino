#include <Wire.h>
#include "MPU6050.h"
#include <SoftwareSerial.h>

MPU6050 mpu;

SoftwareSerial communication(2, 3);

unsigned long timer = 0;
float timeStep = 0.03;

int yaw = 0;
float prev_time = 0;

void sendIntSerial(int x) {
  if (millis() - prev_time > 700) {
    uint8_t LSB = x;
    uint8_t MSB = x >> 8;
    communication.write(MSB);
    communication.write(LSB);

    prev_time = millis();
    Serial.print("[SENT] ");
    Serial.println(x);
  }
}

void setup() {
  Serial.begin(9600);
  communication.begin(9600);
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  mpu.calibrateGyro();
  mpu.setThreshold(3);
}

void loop() {
  timer = millis();

  Vector norm = mpu.readNormalizeGyro();

  int tempYaw = yaw + norm.ZAxis * timeStep;
  if (tempYaw > 360) {
    yaw = 0;
  }
  else if (tempYaw < 0) {
    yaw = 360;
  }
  else {
    yaw = yaw + norm.ZAxis * timeStep;
  }

  sendIntSerial(yaw);

  Serial.print(" Yaw = ");
  Serial.println(yaw);

  delay((timeStep * 1000) - (millis() - timer));
}
