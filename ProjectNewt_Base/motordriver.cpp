#include "motordriver.h"

//Whenever the class is called the constructor will set mode of the pins defined in the object
MotorDriver::MotorDriver(int enablePin, int in1, int in2){
  _enablePin = enablePin;
  _in1 = in1;
  _in2 = in2;

  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_enablePin, OUTPUT);
};

void MotorDriver::begin(){
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_enablePin, OUTPUT);
};

void MotorDriver::forward(int speed){
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, LOW);
  analogWrite(_enablePin, speed);

};

void MotorDriver::reverse(int speed){
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, HIGH);
  analogWrite(_enablePin, speed);
};

void MotorDriver::stop(){
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  analogWrite(_enablePin, 0);
};


