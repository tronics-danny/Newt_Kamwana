#include <Arduino.h>
#include "motordriver.h"
#include "definitions.h"
#include "servos.h"
#include <esp_now.h>
#include <WiFi.h>

//Global Variables
int mapX1, mapY1;
bool button_state1 ;

int mapX2, mapY2;
bool button_state2 ;

//Servo angle variable
int servo1Angle, servo2Angle;


// Motor A and B instances
MotorDriver motorA(ENA, IN1, IN2);
MotorDriver motorB(ENB, IN3, IN4);

// Structure to receive joystick data
// Must match the sender structure
typedef struct struct_message {
  int mappedX1;
  int mappedY1;
  bool buttonState1;

  int mappedX2;
  int mappedY2;
  bool buttonState2;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  //On receiving assign the received data to variables
  mapX1 = myData.mappedX1;
  mapY1 = myData.mappedY1;
  button_state1 = myData.buttonState1;

  mapX2 = myData.mappedX2;
  mapY2 = myData.mappedY2;
  button_state2 = myData.buttonState2;
  
  /*Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("mappedX1: ");
  Serial.println(mapX1);
  Serial.print("mappedY1: ");
  Serial.println(mapY1);
  Serial.print("Button State1: ");
  Serial.println(button_state1);
  Serial.println();*/

  Serial.print("mappedX2: ");
  Serial.println(mapX2);
  Serial.print("mappedY2: ");
  Serial.println(mapY2);
  Serial.print("Button State2: ");
  Serial.println(button_state2);
  Serial.println();
}


void setup() {
  //initialize serial monitor
  Serial.begin(115200);

  ///////////////////////////////////////////////////
  // Setting up for ESP_Now Receiving
  ///////////////////////////////////////////////////
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  ///////////////////////////////////////////////////

  motorA.begin();
  motorB.begin();

  // start servos
  setupServos();
}

void loop() {
  //Light an LED if button on the joy stick is pressed
  if (button_state2 == 0){
    digitalWrite(LEDPIN, HIGH);
  }
  else{
    digitalWrite(LEDPIN, LOW);
  }

  // Map Received joy values to 0 to 180 degrees for servo rotation
  servo1Angle = map(mapX2, -255, 255, 0, 180);
  servo2Angle = map(mapY2, -255, 255, 0, 180);

  // Turn servo motor
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);


  // Differential Drive Logic 
  int leftMotorSpeed  = mapY1 + mapX1;  // Forward/reverse + turning
  int rightMotorSpeed = mapY1 - mapX1;  // Forward/reverse - turning

  // Clamp speeds to motor range [-255, 255]
  leftMotorSpeed  = constrain(leftMotorSpeed, -255, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, -255, 255);

  //Serial.print("leftMotorSpeed: "); Serial.print(leftMotorSpeed);
  //Serial.print("  rightMotorSpeed: "); Serial.println(rightMotorSpeed);

  // Drive motors
  if (leftMotorSpeed > 0){
    motorA.forward(leftMotorSpeed);
  } 
  else if (leftMotorSpeed < 0) {
    motorA.reverse(abs(leftMotorSpeed));
  }
  else {
    motorA.stop();
  }
  
  if (rightMotorSpeed > 0) {
    motorB.forward(rightMotorSpeed);
  }
  else if (rightMotorSpeed < 0) {
    motorB.reverse(abs(rightMotorSpeed));
  }
  else {
    motorB.stop();
  }

  delay(10);


}

