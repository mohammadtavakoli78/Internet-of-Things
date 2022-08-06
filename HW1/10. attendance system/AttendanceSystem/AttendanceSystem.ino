#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(53, 52);

#define RS_PIN 5
#define EN_PIN 4
#define D4_PIN 3
#define D5_PIN 2
#define D6_PIN 1
#define D7_PIN 0
LiquidCrystal lcd(RS_PIN, EN_PIN,D4_PIN, D5_PIN,D6_PIN, D7_PIN );

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pin_rows[ROW_NUM] = {14, 15, 16, 17};
byte pin_column[COLUMN_NUM] = {18, 19, 20};
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

Servo servo;

const int LED_PIN = 22;
const int BUZZER_PIN = 24;
const int SERVO_PIN = 40;

const int ALLOWED_TIME = 10;
const int END_TIME = 20;

String stuNumbers[9] = {"9731014", "9731015", "9731016", "9731017", "9731018", "9731019", "9731020", "9731021", "9731022"};
String presenceList[9];
String absenseList[9];

uint32_t startTime;
String userType = "";
bool isStarted = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  lcd.begin(16, 4);
  servo.attach(SERVO_PIN);
  mySerial.begin(9600);

  servo.write(0);
}

void loop() {
  uint32_t currentTime = millis();
  uint32_t diff = (currentTime - startTime)/1000;
  int arraySize = sizeof(presenceList)/sizeof(String);
  
  char inputKey = keypad.getKey();

  printOnLcd(0, userType);
  
  mySerial.println(diff);

  if(diff > END_TIME) {
    printList();
    exit(0);
  }
  
  if(userType == "1*" && !isStarted) {
    isStarted = true;
    startTime = millis();
    lcd.clear();
    userType = "";
  } else if(inputKey == '#') {
    printOnLcd(0, "ID: " + userType);
    if(diff <= ALLOWED_TIME) {
      digitalWrite(LED_PIN, HIGH);
      servo.write(180);
      delay(3000);
      digitalWrite(LED_PIN, LOW);
      servo.write(0);
      for(int i=0; i<arraySize; ++i) {
        if(presenceList[i] == "") {
          presenceList[i] = userType;
          userType = "";
        }
      }
    } else {
      printOnLcd(1, "ElapsedTime: " + String(diff));
      tone(BUZZER_PIN, 1000);
      delay(1000);
      noTone(BUZZER_PIN);
      for(int i=0; i<arraySize; ++i) {
        if(absenseList[i] == "") {
          absenseList[i] == userType;
          userType = "";
        }
      }
    }
    lcd.clear();
  } else {
    userType += String(inputKey);  
  }
}

void printOnLcd(int lineNo, String Message) {
  lcd.setCursor(0, lineNo);
  lcd.print(Message);
}

void printList() {
  mySerial.println();
  mySerial.println();
  mySerial.println("Presence List:");
  for(int i=0; i<sizeof(presenceList)/sizeof(String); ++i) {
    if(presenceList[i] != "") {
      mySerial.println(presenceList[i]);
    }
  }
  mySerial.println("=============");
  mySerial.println("Absence List:");
  for(int i=0; i<sizeof(stuNumbers)/sizeof(String); ++i) {
    if(existInArray(presenceList, stuNumbers[i]) == 0) {
      mySerial.println(stuNumbers[i]);
    }
  }
}

int existInArray(String *input, String element) {
  for(int i=0; i<9; ++i) {
    if(input[i] == element) {
      return 1;
    }  
  }
  return 0;
}
