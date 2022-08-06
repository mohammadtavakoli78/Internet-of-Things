#include <SoftwareSerial.h>

SoftwareSerial mySerial(53, 52);

int selectedTime;
const int SOUNDER_PIN = 25;
String message;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  pinMode(SOUNDER_PIN, OUTPUT);
}

void loop() {
  int currentTime = millis();
  mySerial.println("Elapsed Time: "+String((currentTime - selectedTime)/1000));
  if((currentTime - selectedTime)/1000 > message.toInt() && message.toInt() > 0) {
    tone(SOUNDER_PIN, 100);
  } else {
    noTone(SOUNDER_PIN);
  }
  if (Serial.available() > 0) {
    message = Serial.readString();
    mySerial.println("Input Message: "+message);
    
    if(message == "*") {
      message = "5";
      selectedTime = millis();
    } else if(message == "#") {
      noTone(SOUNDER_PIN);
      exit(0);
    } else {
      selectedTime = millis();
    }
  }
}
