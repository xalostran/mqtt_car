#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <stdlib.h>

#include "sensitive_data.h"

#define CLIENT_ID "ArduinoSubscriber"
#define TOPIC "send/data"
#define QoS 1

WiFiClient arduinoClient;
PubSubClient client(arduinoClient);

const int in1 = 2;
const int in2 = 4;
const int enA = 3;

const int in3 = 5;
const int in4 = 7;
const int enB = 6;

const int led = 13;
int status = WL_IDLE_STATUS;
String receivedCommand = "";

void messageArrivedCallback(char* topic, byte *payload, unsigned int len){
  char message[len + 1];
  memcpy(message, payload, len);
  message[len] = '\0'; // Null-terminera strängen

  printf("Message arrived:\n");
  printf("  Topic: %s\n", topic);
  printf("  Message: %s\n", message);

  // Spara kommandot för bearbetning i loop()
  receivedCommand = String(message);
}

void setup() {

  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);

  
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(led, OUTPUT);

  if(WiFi.status() == WL_NO_MODULE){
    printf("Communication with module failed.\n");
    while (true);
    
  }
  String fv = WiFi.firmwareVersion();
  if(fv < WIFI_FIRMWARE_LATEST_VERSION){
    printf("Update your firmware.\n");
  }

  while (status != WL_CONNECTED)
  {
    printf("Attempting to connect to WPA SSID: %s\n", SSID);
    status = WiFi.begin(SSID, PASSWORD);
    delay(10000);
  }

  printf("You are connected to the network\n");
  digitalWrite(led, HIGH);

  client.setServer(SERVER, PORT);
  client.setCallback(messageArrivedCallback);

  while (!client.connected())
  {
    printf("Connecting to MQTT server...\n");
    if(client.connect(CLIENT_ID)){
      printf("Connected");
      client.subscribe(TOPIC, QoS);
    }else{
      printf("failed, rc=");
      printf("%d", client.state());
      delay(5000);
    }
  }

}

void loop() {

  client.loop();
  // put your main code here, to run repeatedly:

  if(receivedCommand == "forward"){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 100);
    analogWrite(enB, 100);

  }else if(receivedCommand == "stop"){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, 100);
    analogWrite(enB, 100);
  }

  else if(receivedCommand == "backward"){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, 100);
    analogWrite(enB, 100);
    
  }
  else if(receivedCommand == "right"){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, 100);
    analogWrite(enB, 100);
  }
  else if(receivedCommand == "left"){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 100);
    analogWrite(enB, 100);
  }

  receivedCommand = "";
}

