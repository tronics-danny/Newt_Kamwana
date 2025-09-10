#ifndef SERVOS_H
#define SERVOS_H

#include <Arduino.h>
#include <ESP32Servo.h>

extern Servo servo1;
extern Servo servo2;

void setupServos();

#endif