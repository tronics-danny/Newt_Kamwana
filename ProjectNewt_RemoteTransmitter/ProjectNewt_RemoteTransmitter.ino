#include <esp_now.h>
#include <WiFi.h>
#include "joy.h"
#include "definitions.h"


// RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xd0, 0xef, 0x76, 0x44, 0xc3, 0xfc};


//Global Variables
//Motor control servo
int mapX1, mapY1, rawX1, rawY1, pos_x1, pos_y1;
bool button_state1;

//Servo control servo
int mapX2, mapY2, rawX2, rawY2, pos_x2, pos_y2;
bool button_state2;

// JoyStick Instances
JoyStick joystick(VRX1, VRY1, SW1);
JoyStick ServoJoystick(VRX2, VRY2, SW2);

// Structure to send the joystick data
// Must match the receiver structure
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

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Get readings from the motor control joystick 
  rawX1 = joystick.getVRX();
  rawY1 = joystick.getVRY();
  button_state1 = joystick.getButtonState();

  // Get readings from the servo control joystick 
  rawX2 = ServoJoystick.getVRX();
  rawY2 = ServoJoystick.getVRY();
  button_state2 = ServoJoystick.getButtonState();

  // target = map(source, lowSource, highSource, lowTarget, highTarget)

  // Mapping the raw value first within -500 to 500 range
  pos_x1 = map(rawX1, 0, 4095, -500, 500);
  pos_y1 = map(rawY1, 0, 4095, -500, 500);

  // Mapping the raw value of the servo control joystick 
  // first within -500 to 500 range
  pos_x2 = map(rawX2, 0, 4095, -500, 500);
  pos_y2 = map(rawY2, 0, 4095, -500, 500);

  //Here we are getting out mapped value and remapping it to 
  //another range but this time applying the deadzone
  mapX1 = joystick.applyDeadzoneAndMap(pos_x1, 60);
  mapY1 = joystick.applyDeadzoneAndMap(pos_y1, 60);

  mapX2 = ServoJoystick.applyDeadzoneAndMap(pos_x2, 60);
  mapY2 = ServoJoystick.applyDeadzoneAndMap(pos_y2, 60);

  // Set values to send over the ESP_NOW channel
  myData.mappedX1 = mapX1;
  myData.mappedY1 = mapY1;
  myData.buttonState1 = button_state1;

  myData.mappedX2 = mapX2;
  myData.mappedY2 = mapY2;
  myData.buttonState2 = button_state2;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    //Serial.print("Button state: "); Serial.println(button_state2);
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10);
}