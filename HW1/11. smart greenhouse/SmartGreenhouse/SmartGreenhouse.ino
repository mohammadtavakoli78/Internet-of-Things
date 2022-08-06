#include <Servo.h>
#include <Keypad.h>

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pin_rows[ROW_NUM] = {5, 4, 3, 2};
byte pin_column[COLUMN_NUM] = {8, 7, 6};
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

Servo servo1;
Servo servo2;
Servo servo3;

const int SERVO1_PIN = 9;
const int SERVO2_PIN = 10;
const int SERVO3_PIN = 11;
const int SOUNDER_PIN = 13;
const int FLAME_PIN = 12;
const int SOIL1_PIN = A0;
const int SOIL2_PIN = A1;
const int SOIL3_PIN = A2;

bool flame = false;
String userType;

int numRow;
int degree;

bool isServoOpen1 = false;
bool isServoOpen2 = false;
bool isServoOpen3 = false;

void setup() {
  pinMode(SOUNDER_PIN, OUTPUT);
  pinMode(SOIL1_PIN, INPUT);
  pinMode(SOIL2_PIN, INPUT);
  pinMode(SOIL3_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  Serial.begin(9600);
}

void loop() {
  readKeypad();
  detectFlame();
  
  int soildValue1 = map(analogRead(SOIL1_PIN), 0, 1023, 0, 180);
  int soildValue2 = map(analogRead(SOIL2_PIN), 0, 1023, 0, 180);
  int soildValue3 = map(analogRead(SOIL3_PIN), 0, 1023, 0, 180);

  if(!flame) {
    if(!isServoOpen1) {
      servo1.write(soildValue1);
    }
    if(!isServoOpen2) {
      servo2.write(soildValue2);
    }
    if(!isServoOpen3) {
      servo3.write(soildValue3);
    }
  }
}

void printOnLcd(String Message) {
  Serial.println(Message);
}

void detectFlame() {
  if(digitalRead(FLAME_PIN) == HIGH) {
    flame = true;
    tone(SOUNDER_PIN, 100);
    servo1.write(180);
    servo2.write(180);
    servo3.write(180);
    delay(2000);
    noTone(SOUNDER_PIN);
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    flame = false;
  }
}

void readKeypad() {
  Serial.println(userType);
  char inputKey = keypad.getKey();
  if(inputKey == '*') {
    numRow = userType.toInt();
    userType = "";
  } else if(inputKey == '#') {
    degree = userType.toInt();
    if(numRow == 1) {
      servo1.write(degree);
      isServoOpen1 = true;
    } else if(numRow == 2) {
      servo2.write(degree);
      isServoOpen2 = true;
    } else {
      servo3.write(degree);
      isServoOpen3 = true;
    }
    userType = "";
  } else {
    userType += String(inputKey);
  }
}
