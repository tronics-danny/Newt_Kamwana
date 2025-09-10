#include "servos.h"
#include "definitions.h"

Servo servo1;
Servo servo2;

void setupServos() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
}
