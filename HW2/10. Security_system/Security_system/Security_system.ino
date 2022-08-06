#include <SoftwareSerial.h>
#include <BlynkSimpleStream.h>
#include <Servo.h>

SoftwareSerial mySerial(53, 52);

#define BLYNK_TEMPLATE_ID "TMPLqyaEyTdv"
#define BLYNK_DEVICE_NAME "SecuritySystem"
#define BLYNK_AUTH_TOKEN "41bF76Iw8GOHHV9HinRvW578cKLRWskJ"

#define BLYNK_PRINT Serial1

char auth[] = BLYNK_AUTH_TOKEN;

Servo servo;

const int SERVO_PORT = 40;
const int LED_RED_PIN = 22;
const int SOUNDER_PORT = 30;

const int pingPin = 45;
const int echoPin = 46; 

int distance;
int cameraDegree;
int securitySystemOn;
bool ownerConfig = false;

BlynkTimer timer;

void myTimerEvent() {
  Blynk.virtualWrite(V0, distance);
}

BLYNK_WRITE(V1)
{
  cameraDegree = param.asInt();
  mySerial.println("Camera Degree is: "+String(cameraDegree));

  int servoDegree = cameraDegree;
  mySerial.println("Servo Degree is: "+String(servoDegree));
  servo.write(servoDegree);
}

BLYNK_WRITE(V2)
{
  securitySystemOn = param.asInt();
  if(securitySystemOn == 0) {
    mySerial.println("security System is: Off");
  } else if(securitySystemOn == 1) {
    mySerial.println("security System is: On");
  }
  ownerConfig = true;
  changeAlert(securitySystemOn);
}

void setup()
{
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(SOUNDER_PORT, OUTPUT);
  servo.attach(SERVO_PORT);
  servo.write(0);
    
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

  int temp = getDistance();
  if(temp > 0) {
    distance = temp;
  }

  if(distance < 10 && !ownerConfig) {
    changeAlert(1);
  } else {
    changeAlert(0);
  }
}

int getDistance() {
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  digitalWrite(pingPin, HIGH);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  int localDistance = duration * 0.034 / 2;

  return localDistance;
}

void changeAlert(int mode) {
  if(mode == 0) {
    digitalWrite(LED_RED_PIN, LOW);
    noTone(SOUNDER_PORT);
  } else if(mode == 1) {
    digitalWrite(LED_RED_PIN, HIGH);
    tone(SOUNDER_PORT, 1000);
  }
}
