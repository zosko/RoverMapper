#include <Arduino.h>
#include "StepperMotor.h"

StepperMotor::StepperMotor(int In1, int In2, int In3, int In4, int In5, int In6, int In7, int In8) {
  // Record pin numbers in the inputPins array
  this->inputPins[0] = In1;
  this->inputPins[1] = In2;
  this->inputPins[2] = In3;
  this->inputPins[3] = In4;

  this->inputPins1[0] = In5;
  this->inputPins1[1] = In6;
  this->inputPins1[2] = In7;
  this->inputPins1[3] = In8;

  // Iterate through the inputPins array, setting each one to output mode
  for (int inputCount = 0; inputCount < 4; inputCount++) {
    pinMode(this->inputPins[inputCount], OUTPUT);
  }

  for (int inputCount = 0; inputCount < 4; inputCount++) {
    pinMode(this->inputPins1[inputCount], OUTPUT);
  }
  duration = 50;
}

void StepperMotor::setStepDuration(int duration) {
  this->duration = duration;
}

void StepperMotor::forward(int noOfSteps) {
  /*
      The following 2D array represents the sequence that must be
      used to acheive rotation. The rows correspond to each step, and
      the columns correspond to each input. L
  */
  bool sequence[][4] = {{LOW, LOW, LOW, HIGH },
    {LOW, LOW, HIGH, HIGH},
    {LOW, LOW, HIGH, LOW },
    {LOW, HIGH, HIGH, LOW},
    {LOW, HIGH, LOW, LOW },
    {HIGH, HIGH, LOW, LOW},
    {HIGH, LOW, LOW, LOW },
    {HIGH, LOW, LOW, HIGH}
  };

  int factor = abs(noOfSteps) / noOfSteps;    // If noOfSteps is +, factor = 1. If noOfSteps is -, factor = -1
  noOfSteps = abs(noOfSteps);    // If noOfSteps was in fact negative, make positive for future operations

  /*
      The following algorithm runs through the sequence the specified number
      of times
  */
  for (int sequenceNum = 0;  sequenceNum <= noOfSteps / 8; sequenceNum++) {
    for (int position = 0; ( position < 8 ) && ( position < ( noOfSteps - sequenceNum * 8 )); position++) {
      delay(duration);
      for (int inputCount = 0; inputCount < 4; inputCount++) {
        digitalWrite(this->inputPins[inputCount], sequence[(int)(3.5 - (3.5 * factor) + (factor * position))][inputCount]);
        digitalWrite(this->inputPins1[inputCount], sequence[(int)(3.5 - (3.5 * factor) + (factor * position))][inputCount]);
      }
    }
  }
}

void StepperMotor::backward(int noOfSteps) {
  /*
      The following 2D array represents the sequence that must be
      used to acheive rotation. The rows correspond to each step, and
      the columns correspond to each input. L
  */

  noOfSteps = -noOfSteps;

  bool sequence[][4] = {{LOW, LOW, LOW, HIGH },
    {LOW, LOW, HIGH, HIGH},
    {LOW, LOW, HIGH, LOW },
    {LOW, HIGH, HIGH, LOW},
    {LOW, HIGH, LOW, LOW },
    {HIGH, HIGH, LOW, LOW},
    {HIGH, LOW, LOW, LOW },
    {HIGH, LOW, LOW, HIGH}
  };

  int factor = abs(noOfSteps) / noOfSteps;    // If noOfSteps is +, factor = 1. If noOfSteps is -, factor = -1
  noOfSteps = abs(noOfSteps);    // If noOfSteps was in fact negative, make positive for future operations

  /*
      The following algorithm runs through the sequence the specified number
      of times
  */
  for (int sequenceNum = 0;  sequenceNum <= noOfSteps / 8; sequenceNum++) {
    for (int position = 0; ( position < 8 ) && ( position < ( noOfSteps - sequenceNum * 8 )); position++) {
      delay(duration);
      for (int inputCount = 0; inputCount < 4; inputCount++) {
        digitalWrite(this->inputPins[inputCount], sequence[(int)(3.5 - (3.5 * factor) + (factor * position))][inputCount]);
        digitalWrite(this->inputPins1[inputCount], sequence[(int)(3.5 - (3.5 * factor) + (factor * position))][inputCount]);
      }
    }
  }
}

void StepperMotor::left(int noOfSteps, int noOfSteps1) {
  /*
      The following 2D array represents the sequence that must be
      used to acheive rotation. The rows correspond to each step, and
      the columns correspond to each input. L
  */
  bool sequence[][4] = {{LOW, LOW, LOW, HIGH },
    {LOW, LOW, HIGH, HIGH},
    {LOW, LOW, HIGH, LOW },
    {LOW, HIGH, HIGH, LOW},
    {LOW, HIGH, LOW, LOW },
    {HIGH, HIGH, LOW, LOW},
    {HIGH, LOW, LOW, LOW },
    {HIGH, LOW, LOW, HIGH}
  };

  int factor = abs(noOfSteps) / noOfSteps;    // If noOfSteps is +, factor = 1. If noOfSteps is -, factor = -1
  noOfSteps = abs(noOfSteps);    // If noOfSteps was in fact negative, make positive for future operations

  noOfSteps1 = -noOfSteps1;
  int factor1 = abs(noOfSteps1) / noOfSteps1;    // If noOfSteps is +, factor = 1. If noOfSteps is -, factor = -1
  noOfSteps1 = abs(noOfSteps1);    // If noOfSteps was in fact negative, make positive for future operations

  /*
      The following algorithm runs through the sequence the specified number
      of times
  */
  for (int sequenceNum = 0;  sequenceNum <= noOfSteps / 8; sequenceNum++) {
    for (int position = 0; ( position < 8 ) && ( position < ( noOfSteps - sequenceNum * 8 )); position++) {
      delay(duration);
      for (int inputCount = 0; inputCount < 4; inputCount++) {
        digitalWrite(this->inputPins[inputCount], sequence[(int)(3.5 - (3.5 * factor) + (factor * position))][inputCount]);
        digitalWrite(this->inputPins1[inputCount], sequence[(int)(3.5 - (3.5 * factor1) + (factor1 * position))][inputCount]);
      }
    }
  }
}

void StepperMotor::right(int noOfSteps, int noOfSteps1) {
  /*
      The following 2D array represents the sequence that must be
      used to acheive rotation. The rows correspond to each step, and
      the columns correspond to each input. L
  */
  bool sequence[][4] = {{LOW, LOW, LOW, HIGH },
    {LOW, LOW, HIGH, HIGH},
    {LOW, LOW, HIGH, LOW },
    {LOW, HIGH, HIGH, LOW},
    {LOW, HIGH, LOW, LOW },
    {HIGH, HIGH, LOW, LOW},
    {HIGH, LOW, LOW, LOW },
    {HIGH, LOW, LOW, HIGH}
  };

  noOfSteps = -noOfSteps;

  int factor = abs(noOfSteps) / noOfSteps;    // If noOfSteps is +, factor = 1. If noOfSteps is -, factor = -1
  noOfSteps = abs(noOfSteps);    // If noOfSteps was in fact negative, make positive for future operations

  int factor1 = abs(noOfSteps1) / noOfSteps1;    // If noOfSteps is +, factor = 1. If noOfSteps is -, factor = -1
  noOfSteps1 = abs(noOfSteps1);    // If noOfSteps was in fact negative, make positive for future operations

  /*
      The following algorithm runs through the sequence the specified number
      of times
  */
  for (int sequenceNum = 0;  sequenceNum <= noOfSteps / 8; sequenceNum++) {
    for (int position = 0; ( position < 8 ) && ( position < ( noOfSteps - sequenceNum * 8 )); position++) {
      delay(duration);
      for (int inputCount = 0; inputCount < 4; inputCount++) {
        digitalWrite(this->inputPins[inputCount], sequence[(int)(3.5 - (3.5 * factor) + (factor * position))][inputCount]);
        digitalWrite(this->inputPins1[inputCount], sequence[(int)(3.5 - (3.5 * factor1) + (factor1 * position))][inputCount]);
      }
    }
  }
}

