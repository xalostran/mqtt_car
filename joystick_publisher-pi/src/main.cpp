#include <Arduino.h>
#include <stdlib.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

#include "sensitive_info.h"

const int vX = A1;
const int vY = A0;
const int sW = 22;

const int data_led = 10;
const int wifi_led = 11;
const int mqtt_led = 12;
// put function declarations here:

int xVal;
int yVal;
int buttonState;

String sendCommand = "";

#define CLIENT_ID "ArduinoPublisher"
#define TOPIC "send/data"
#define QoS 1

int status = WL_IDLE_STATUS;

WiFiClient raspberryClient;
PubSubClient client(raspberryClient);

void setup() {
  // put your setup code here, to run once:
  pinMode(vX, INPUT);
  pinMode(vY, INPUT);
  pinMode(sW, INPUT_PULLUP);
  pinMode(data_led, OUTPUT);
  pinMode(wifi_led, OUTPUT);
  pinMode(mqtt_led, OUTPUT);

  if(WiFi.status() == WL_NO_MODULE){
    digitalWrite(wifi_led, HIGH);
    delay(100);
    digitalWrite(wifi_led, LOW);
    delay(100);

    while (true);
  }
  while(status != WL_CONNECTED){
    digitalWrite(wifi_led, HIGH);
    delay(1000);
    digitalWrite(wifi_led, LOW);
    delay(1000);
    status = WiFi.begin(SSID, PASS);
  }

  digitalWrite(wifi_led, HIGH);

  client.setServer(IP, PORT);
  
  while (!client.connected())
  {
    digitalWrite(mqtt_led, HIGH);
    delay(1000);
    digitalWrite(mqtt_led, LOW);
    delay(1000);
    if(client.connect(CLIENT_ID)){
      digitalWrite(mqtt_led, HIGH);
    }
  }
  
}

void loop() {
  client.loop();

  xVal = analogRead(vX);
  yVal = analogRead(vY);
  buttonState = digitalRead(sW);

  if(xVal >= 501 && xVal <= 514 && yVal >= 501 && yVal <= 514){
    """The values in this if statement may vary depending on 
    how stable your microcontroller recieves voltage.
     For me, the values could shift between 501 - 514 while not touching the joystick.
    Log your values in the serial monitor to ensure exactly 
      what values you should write in the if statement."""

    sendCommand = "stop";
  }
  else if(xVal > 514){
    sendCommand = "forward";
  }
  else if(xVal < 501){
    sendCommand = "backward";
  }
  else if(yVal > 1000){
    sendCommand = "right";
  }
  else if(yVal < 10){
    sendCommand = "left";
  }
  if(sendCommand != ""){
    client.publish(TOPIC, sendCommand.c_str());
  }
  delay(500);

  /*
  y 514 - 1023 = right
  y 501 - 0 = left
  x 514 - 1023 = up
  x 501 - 0 = down
  */
}
