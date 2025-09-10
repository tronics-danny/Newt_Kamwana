#include "joy.h"

JoyStick::JoyStick(int vrX, int vrY, int button){
  _vrX = vrX;
  _vrY = vrY;
  _button = button;

}

void JoyStick::begin(){
  //Here we set the mode of the joystick pins
  pinMode(_vrX, INPUT);
  pinMode(_vrY, INPUT);
  pinMode(_button, INPUT_PULLUP);

}

int JoyStick::getVRX(){
  //Return an interger value of the vrX reading
  return analogRead(_vrX);

}

int JoyStick::getVRY(){
  //Return an interger value of the vrY reading
  return analogRead(_vrY);

}

int JoyStick::applyDeadzoneAndMap(int val, int deadzone) {
  int center = 0;  // because we mapped to -500..500

  // inside deadzone → snap to 0
  if (val > (center - deadzone) && val < (center + deadzone)) {
    return 0;
  }

  // left side
  if (val < center - deadzone) {
    return map(val, -500, center - deadzone, -255, 0);
  }

  // right side
  return map(val, center + deadzone, 500, 0, 255);
}

bool JoyStick::getButtonState(){
  //Return state of button
  return digitalRead(_button);
}