#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1);

const int LED_PORT = 9;

const int LDR_PORT = A0;

void setup() {
  mySerial.begin(9600);
  
  pinMode(LED_PORT, OUTPUT);
  pinMode(LDR_PORT, INPUT);
}

void loop() {
  int ldrStatus = analogRead(LDR_PORT);
  int volt = map(ldrStatus, 11, 990, 255, 0);
  mySerial.println(String(ldrStatus) + " degree, " + String(volt) + " volt");
  analogWrite(LED_PORT, volt);
}
