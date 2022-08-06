#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1);

Servo servo;
const int SERVO_PORT = 13;

const int pingPin = 7;
const int echoPin = 6; 

long openDoor;
bool isOpen = false;

void setup() {
  mySerial.begin(9600);
  servo.attach(SERVO_PORT);
  servo.write(0);
}

void loop() {
  long duration, inches, cm;
  int openTime = 0;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  digitalWrite(pingPin, HIGH);
  
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  if(distance <= 100) {
    openTime = (millis() - openDoor) / 1000;
    mySerial.print(openTime);
    mySerial.print("s ");
    mySerial.println();
    if(openTime > 5) {
      servo.write(90);
    } else {
      servo.write(0);
      isOpen = true;
    }
    if(!isOpen) {
      openDoor = millis();
    }
  }
  if(distance > 100) {
    isOpen = false;
    servo.write(0);
    openDoor = millis();
  }

  if(duration > 0) {
    mySerial.print(distance);
    mySerial.print("cm");
    mySerial.println();
  }
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
