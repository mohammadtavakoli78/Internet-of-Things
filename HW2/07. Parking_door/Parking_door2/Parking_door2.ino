#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(53, 52);

Servo servo;
const int SERVO_PORT = 30;

const int LED_RED_PIN = 22;
const int pingPin = 46;
const int echoPin = 47; 

long openDoor;
long closeDoor;
bool isOpen = false;
bool isClose = true;
bool duringClose = false;
int distance;

void setup() {
  pinMode(LED_RED_PIN, OUTPUT);

  Serial.begin(9600);
  mySerial.begin(9600);
    
  servo.attach(SERVO_PORT);
  servo.write(0);
}

void loop() {
  int localDistance = getDistance(); 
  if(localDistance > 0) {
    distance = localDistance;
  }
  
  closeDoor = millis();
  if (isOpen) {
    if((millis() - openDoor) / 1000 > 10) {
      mySerial.println("distance: "+String(distance));
      decide("3");
    } 
  }
  if (Serial.available() > 0) {
    String message = Serial.readString();
    mySerial.println("Received Message: "+message);
    decide(message);
  }
  if(duringClose) {
    if(distance <= 50) {
      decide("1");
    }
  }
}

int getDistance() {
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  digitalWrite(pingPin, HIGH);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

void decide(String text) {
  if (text == "1") {
    isOpen = true;
    isClose = false;
    digitalWrite(LED_RED_PIN, HIGH);
    for(int i=0; i<=90; ++i) {
      servo.write(i);
    }
    openDoor = millis();
    delay(500);
    digitalWrite(LED_RED_PIN, LOW);
  } else if (text == "3") {
    distance = getDistance();
    if(distance > 50) {
      duringClose = true;
      isOpen = false;
      isClose = true;
      int degree = servo.read();
      digitalWrite(LED_RED_PIN, HIGH);
      for(int i=degree; i>=0; --i) {
        servo.write(i);
      }
      delay(500);
      duringClose = false;
      digitalWrite(LED_RED_PIN, LOW);
    }
  }
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
