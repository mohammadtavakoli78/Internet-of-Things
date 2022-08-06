#include <SoftwareSerial.h>

SoftwareSerial mySerial(53, 52);

void setup() {
  mySerial.begin(9600);
  mySerial.println("Name: Mohammad Tavakoli");
  mySerial.println("Sutdent Number: 9731014");
}

void loop() {
}
