#include "Ultrasonic.h"
#include "L298N.h"

Ultrasonic ultrasonic_Left(52, 53);
Ultrasonic ultrasonic_Center(44, 45);
Ultrasonic ultrasonic_Right(32, 33);

int heading = 0;
int target_heading = 0;
int target_trashold = 5;
bool canTakeTargetHeading = true;

const int ENA = 6;
const int IN1 = 8;
const int IN2 = 7;
const int IN3 = 2;
const int IN4 = 4;
const int ENB = 3;

L298N driver(ENA, IN1, IN2, IN3, IN4, ENB);
int time_delay = 100;
int speed = 100;

int left_eye = 0;
int center_eye = 0;
int right_eye = 0;

int distance_colision = 20;

void setup() {
  Serial.begin(9600);
  Serial1.begin(57600);
  Serial3.begin(9600);
}
void printDistance() {
  if (heading >= 0 && heading <= 360) {
    Serial.print(" [HEADING] ");
    Serial.print(heading);

    Serial.print("  [EYE] ");

    Serial.print(left_eye);
    Serial.print(" -- ");

    Serial.print(center_eye);
    Serial.print(" -- ");

    Serial.print(right_eye);
    Serial.println("");
  }

}
void getDistances() {
  if (Serial3.available()) {
    uint8_t MSB = Serial3.read();
    uint8_t LSB = Serial3.read();
    heading = (MSB << 8) | LSB;
  }

  left_eye = ultrasonic_Left.distanceRead();
  center_eye = ultrasonic_Center.distanceRead();
  right_eye = ultrasonic_Right.distanceRead();

  // if (!checkColision()) {
  Serial1.println(heading);
  // }

  printDistance();
}
bool checkColision() {
  if ((center_eye < distance_colision && center_eye != 0) ||
      (left_eye < distance_colision && left_eye != 0) ||
      (right_eye < distance_colision  && right_eye != 0)) {
    return true;
  }
  return false;
}
bool isFullColision() {
  if ((center_eye < distance_colision && center_eye != 0) &&
      (left_eye < distance_colision && left_eye != 0) &&
      (right_eye < distance_colision  && right_eye != 0)) {
    return true;
  }
  return false;
}
bool isLeftColision() {
  if (left_eye < distance_colision && left_eye != 0) {
    return true;
  }
  return false;
}
bool isRightColision() {
  if (right_eye < distance_colision && right_eye != 0) {
    return true;
  }
  return false;
}
bool isCenterColision() {
  if (center_eye < distance_colision && center_eye != 0) {
    return true;
  }
  return false;
}
bool isHeadingGood() {
  int tempLeftMargin = target_heading + target_trashold;
  int tempRightMargin = target_heading - target_trashold;
  if (heading < tempLeftMargin && heading > tempRightMargin) {
    return true;
  }
  else {
    return false;
  }
}
bool isHeadingTooRight() {
  int tempLeftMargin = target_heading - target_trashold;
  if (tempLeftMargin < heading) {
    return true;
  }
  else {
    return false;
  }
}
bool isHeadingTooLeft() {
  int tempLeftMargin = target_heading + target_trashold;
  if (tempLeftMargin > heading) {
    return true;
  }
  else {
    return false;
  }
}
void loop() {
  if (checkColision()) {
    driver.full_stop(time_delay);
    canTakeTargetHeading = true;
    //    Serial1.println("c");
    //    Serial.println("c");
    if (isLeftColision()) {
      while (isLeftColision()) {
        driver.turn_right(speed, time_delay);
        getDistances();
      }
    }
    else if (isRightColision()) {
      while (isRightColision()) {
        driver.turn_left(speed, time_delay);
        getDistances();
      }
    }
    else if (isCenterColision()) {
      if (left_eye > right_eye) {
        while (isCenterColision()) {
          driver.turn_left(speed, time_delay);
          getDistances();
        }
      }
      else {
        while (isCenterColision()) {
          driver.turn_right(speed, time_delay);
          getDistances();
        }
      }
    }
    else if (isFullColision()) {
      while (isFullColision()) {
        driver.backward(speed, time_delay);
        getDistances();
      }
    }
  }
  else {
    getDistances();
    driver.forward(speed, time_delay);

    //    if (canTakeTargetHeading) {
    //      target_heading = heading;
    //      canTakeTargetHeading = false;
    //    }
    //    if (isHeadingGood()) {
    //      driver.forward(speed, time_delay);
    //      Serial.println("forward");
    //    }
    //    else {
    //      if (isHeadingTooLeft()) {
    //        Serial.println("too left");
    //        driver.turn_right(80, 60);
    //      }
    //      else if (isHeadingTooRight()) {
    //        Serial.println("too right");
    //        driver.turn_left(80, 60);
    //      }
    //    }
  }
}
