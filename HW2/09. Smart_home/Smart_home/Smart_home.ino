#include <SoftwareSerial.h>
#include <BlynkSimpleStream.h>
#include <Servo.h>

SoftwareSerial mySerial(53, 52);

#define BLYNK_TEMPLATE_ID           "TMPLkBM0I-4P"
#define BLYNK_DEVICE_NAME           "SmartHome"
#define BLYNK_AUTH_TOKEN            "wiooAWaTy7e5bPGjSVDVxBLX-erpTgtQ"

#define BLYNK_PRINT Serial1

Servo servo;

const int SERVO_PORT = 30;
const int LED_RED_PIN = 22;
const int TEMPERATURE_PORT = A0;

char auth[] = BLYNK_AUTH_TOKEN;
int controlTemp;
int temp;

BlynkTimer timer;

void myTimerEvent() {
  Blynk.virtualWrite(V1, temp);
}

BLYNK_WRITE(V0)
{
  controlTemp = param.asInt();
  mySerial.println("control Temp is: "+String(controlTemp));

  int servoDegree = map(controlTemp, 0, 150, 0, 180);
  mySerial.println("Servo Degree is: "+String(controlTemp));
  servo.write(servoDegree);
}

void setup()
{
  pinMode(LED_RED_PIN, OUTPUT);

  servo.attach(SERVO_PORT);
  servo.write(0);
  
  analogReference(INTERNAL1V1);
  
  mySerial.begin(9600);

  Serial1.begin(115200);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();

  int analogValue = analogRead(TEMPERATURE_PORT);
  temp = analogValue / 9.35;

  if(controlTemp > 0 && temp < controlTemp - 10) {
    digitalWrite(LED_RED_PIN, HIGH);
  } else if(controlTemp > 0 && temp >= controlTemp - 10) {
    digitalWrite(LED_RED_PIN, LOW);
  }
}
