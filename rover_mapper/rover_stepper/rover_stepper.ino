#include "Ultrasonic.h"
#include "StepperMotor.h"

Ultrasonic ultrasonic_Left(52, 53);
Ultrasonic ultrasonic_Center(44, 45);
Ultrasonic ultrasonic_Right(32, 33);

StepperMotor motors(8, 9, 10, 11, 4, 5, 6, 7);
int motor_speed = 1; // motor gets slower as this number increases
int motor_steps = 4076; // 4076 is approx 360 degrees

int heading = 0;
int left_eye = 0;
int center_eye = 0;
int right_eye = 0;
int distance_colision = 20;
int target_heading = 0;
int target_trashold = 5;

void setup() {
  Serial.begin(9600);
  Serial1.begin(57600);
  Serial3.begin(9600);

  motors.setStepDuration(motor_speed);
}
void printDistance() {
  //  if (heading >= 0 && heading <= 360) {
  Serial.print("[ORIENTATION] ");
  Serial.print(heading);

  Serial.print("  [EYE] ");

  Serial.print(left_eye);
  Serial.print(" -- ");

  Serial.print(center_eye);
  Serial.print(" -- ");

  Serial.print(right_eye);
  Serial.println("");

  // if (!checkColision()) {
  Serial1.println(heading);
  // }
  //  }

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
void loop() {
  if (checkColision()) {
    //    Serial1.println("c");
    //    Serial.println("c");
    if (isLeftColision()) {
      while (isLeftColision()) {
        motors.right(motor_steps, motor_steps);
        getDistances();
      }
    }
    else if (isRightColision()) {
      while (isRightColision()) {
        motors.left(motor_steps, motor_steps);
        getDistances();
      }
    }
    else if (isCenterColision()) {
      if (left_eye > right_eye) {
        while (isCenterColision()) {
          motors.left(motor_steps, motor_steps);
          getDistances();
        }
      }
      else {
        while (isCenterColision()) {
          motors.right(motor_steps, motor_steps);
          getDistances();
        }
      }
    }
    else if (isFullColision()) {
      while (isFullColision()) {
        motors.backward(motor_steps);
        getDistances();
      }
    }
  }
  else {
    getDistances();
    motors.forward(motor_steps);
  }
}
