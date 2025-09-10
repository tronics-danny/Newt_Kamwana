#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <Arduino.h>
//Motor header definitions here

class MotorDriver{
  private:
  // Store pin numbers
    int _enablePin, _in1, _in2;

  public:
    //Constructors that defines what first happens when this class is called
    //Here we are passing our contol pins to the constructor
    MotorDriver(int enablePin, int in1, int in2);

    //Here we are defining our methods(functions inside a class)
    void begin();
    void forward(int speed);
    void reverse(int speed);
    void stop();
};

#endif