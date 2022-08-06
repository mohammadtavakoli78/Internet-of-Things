#include <SoftwareSerial.h>

SoftwareSerial mySerial(53, 52);

String password = "1234";

const int LED_RED_PIN = 22;
const int LED_GREEN_PIN = 35;
const int SOUNDER_PIN = 45;

void setup() {
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(SOUNDER_PIN, OUTPUT);

  mySerial.begin(9600);
  Serial.begin(9600);

  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    String message = Serial.readString();
    mySerial.println("Received Password: "+message);
    check(message);
  }
}

int check(String input) {
  if (input == password) {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, HIGH);
    tone(SOUNDER_PIN, 1000);
    delay(3000);
    noTone(SOUNDER_PIN);
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, LOW);
  }
}
