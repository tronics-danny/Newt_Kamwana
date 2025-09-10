#ifndef JOY_H
#define JOY_H

#include <Arduino.h>
//Joy stick header definitions here

class JoyStick{
private:
  //These will be our pins connected to the JoyStick
  int _vrX, _vrY, _button;

public:
  JoyStick(int vrX, int vrY, int button);
  void begin();
  int getVRX();
  int getVRY();
  bool getButtonState();

  //This maps the joy stick readings to values 
  //ranging -255 to 255 for bot x and y axis
  int applyDeadzoneAndMap(int val, int deadzone);

};

#endif