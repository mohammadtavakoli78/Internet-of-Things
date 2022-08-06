#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Keyboard.h>
#include <Servo.h>


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
byte pin_rows[ROW_NUM] = {17, 18, 19, 20};
byte pin_column[COLUMN_NUM] = {14, 15, 16};
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );


Servo servo;
const int SOUNDER_PORT = 42;


String usernames[9] = {"100", "101", "102", "103", "104", "105", "106", "107", "108"};
String passwords[9] = {"123", "124", "125", "126", "127", "128", "129", "130", "131"};
int parkingSpot[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int parkingTime[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};


const int PIR_A1 = 22;
const int PIR_A2 = 25;
const int PIR_A3 = 28;
const int PIR_B1 = 31;
const int PIR_B2 = 34;
const int PIR_B3 = 37;
const int PIR_C1 = 53;
const int PIR_C2 = 50;
const int PIR_C3 = 47;

const int LED_A1 = 23;
const int LED_A2 = 26;
const int LED_A3 = 29;
const int LED_B1 = 32;
const int LED_B2 = 35;
const int LED_B3 = 38;
const int LED_C1 = 52;
const int LED_C2 = 49;
const int LED_C3 = 46;

String userType;
bool entrance = false;
bool isExit = false;
bool enterId = false;
bool enterPass = false;

void setup() {
  pinMode(PIR_A1, INPUT);
  pinMode(PIR_A2, INPUT);
  pinMode(PIR_A3, INPUT);
  pinMode(PIR_B1, INPUT);
  pinMode(PIR_B2, INPUT);
  pinMode(PIR_B3, INPUT);
  pinMode(PIR_C1, INPUT);
  pinMode(PIR_C2, INPUT);
  pinMode(PIR_C3, INPUT);

  pinMode(LED_A1, OUTPUT);
  pinMode(LED_A2, OUTPUT);
  pinMode(LED_A3, OUTPUT);
  pinMode(LED_B1, OUTPUT);
  pinMode(LED_B2, OUTPUT);
  pinMode(LED_B3, OUTPUT);
  pinMode(LED_C1, OUTPUT);
  pinMode(LED_C2, OUTPUT);
  pinMode(LED_C3, OUTPUT);

  lcd.begin(16, 4);

  servo.attach(SOUNDER_PORT);
  servo.write(0);
}

void loop() {
  checkLeds();
  printSpotsInfo();
  readKeypad();
}

void checkLeds() {
  if(digitalRead(PIR_A1) == HIGH) {
    digitalWrite(LED_A1, HIGH);
    parkingSpot[0] = 0;
  } else {
    digitalWrite(LED_A1, LOW);
    parkingSpot[0] = 1;
  }
  if(digitalRead(PIR_A2) == HIGH) {
    digitalWrite(LED_A2, HIGH);
    parkingSpot[1] = 0;
  } else {
    digitalWrite(LED_A2, LOW);
    parkingSpot[1] = 1;
  }
  if(digitalRead(PIR_A3) == HIGH) {
    digitalWrite(LED_A3, HIGH);
    parkingSpot[2] = 0;
  } else {
    digitalWrite(LED_A3, LOW);
    parkingSpot[2] = 1;
  }
  if(digitalRead(PIR_B1) == HIGH) {
    digitalWrite(LED_B1, HIGH);
    parkingSpot[3] = 0;
  } else {
    digitalWrite(LED_B1, LOW);
    parkingSpot[3] = 1;
  }
  if(digitalRead(PIR_B2) == HIGH) {
    digitalWrite(LED_B2, HIGH);
    parkingSpot[4] = 0;
  } else {
    digitalWrite(LED_B2, LOW);
    parkingSpot[4] = 1;
  }
  if(digitalRead(PIR_B3) == HIGH) {
    digitalWrite(LED_B3, HIGH);
    parkingSpot[5] = 0;
  } else {
    digitalWrite(LED_B3, LOW);
    parkingSpot[5] = 1;
  }
  if(digitalRead(PIR_C1) == HIGH) {
    digitalWrite(LED_C1, HIGH);
    parkingSpot[6] = 0;
  } else {
    digitalWrite(LED_C1, LOW);
    parkingSpot[6] = 1;
  }
  if(digitalRead(PIR_C2) == HIGH) {
    digitalWrite(LED_C2, HIGH);
    parkingSpot[7] = 0;
  } else {
    digitalWrite(LED_C2, LOW);
    parkingSpot[7] = 1;
  }
  if(digitalRead(PIR_C3) == HIGH) {
    digitalWrite(LED_C3, HIGH);
    parkingSpot[8] = 0;
  } else {
    digitalWrite(LED_C3, LOW);
    parkingSpot[8] = 1;
  }
}

void printSpotsInfo() {
  int floorA = 0;
  int floorB = 0;
  int floorC = 0;
  int total = 0;
  for(int i=0; i<9; ++i) {
    if(parkingSpot[i]==1) {
      ++total;
    }
    if(i==0 || i==1 || i==2){
      if(parkingSpot[i]==1) {
        ++floorA;
      }
    }
    if(i==3 || i==4 || i==5){
      if(parkingSpot[i]==1) {
        ++floorB;
      }
    }
    if(i==6 || i==7 || i==8){
      if(parkingSpot[i]==1) {
        ++floorC;
      }
    }
  }
  printOnLcd(0, "A: " + String(floorA) + " B: " + String(floorB));
  printOnLcd(1, "C: " + String(floorC) + " Total: " + String(total));
}

void printOnLcd(int lineNo, String Message) {
  lcd.setCursor(0, lineNo);
  lcd.print(Message);
}

void clearOnLcd(int lineNo) {
  lcd.setCursor(0, lineNo);
  lcd.clear();
}

void readKeypad() {
  char inputKey = keypad.getKey();
  printOnLcd(2, String(inputKey));
  if(userType == "1*") {
    entrance = true;
    userType = "";
  }
  if(userType == "2*") {
    isExit = true;
    userType = "";
  }
  if(!entrance && !isExit) {
    userType += String(inputKey);
  }
  if(entrance) {
    printOnLcd(3, userType);
    if(inputKey == '*') {
      if(existInArray(usernames, userType, 0) == 1) {
        userType = "";
        clearOnLcd(3);
      } else if(existInArray(usernames, userType, 0) == 0) {
        clearOnLcd(3);
        userType = "";
        printOnLcd(3, "Username is not exists!");
        entrance = false;
        clearOnLcd(3);  
      } else {
        userType = "";
        clearOnLcd(3);
      }
    } else if(inputKey == '#') {
      if(existInArray(passwords, userType, 1) == 1) {
        entrance = false;
        userType = "";
        servo.write(180);
        delay(1000);
        servo.write(0);
        clearOnLcd(3);
      } else if(existInArray(passwords, userType, 1) == 0) {
        userType = "";
        printOnLcd(3, "Password is not correct!");
        entrance = false;
        clearOnLcd(3);  
      } else {
          userType = "";
          clearOnLcd(3);
        }
    } else {
      userType += String(inputKey);
    }
  }
  if(isExit) {
    printOnLcd(3, userType);
    if(inputKey == '*') {
      if(existInArray(usernames, userType, 0) == 1) {
        userType = "";
        clearOnLcd(3);
      } else if(existInArray(usernames, userType, 0) == 0) {
        clearOnLcd(3);
        userType = "";
        printOnLcd(3, "Username is not exists!");
        entrance = false;
        clearOnLcd(3);  
      } else {
          userType = "";
          clearOnLcd(3);
        }
    } else if(inputKey == '#') {
      if(existInArray(passwords, userType, 2) == 1) {
        isExit = false; 
        userType = "";
        servo.write(180);
        delay(1000);
        servo.write(0);
        clearOnLcd(3);
      } else if(existInArray(usernames, userType, 0) == 0) {
        userType = "";
        printOnLcd(3, "Password is not correct!");
        entrance = false;
        clearOnLcd(3);  
      } else {
          userType = "";
          clearOnLcd(3);
        }
    } else {
      userType += String(inputKey);
    }
  }
}

int existInArray(String *input, String element, int type) {
  for(int i=0; i<9; ++i) {
    if(input[i] == element) {
      if(type == 1) {
        if(parkingSpot[i] == 1) {
          parkingSpot[i] = 0;
          parkingTime[i] = millis();
          reserveSpot(0, i);
        } else {
          printOnLcd(3, "No Space is available");
          return 2;
        }
      }
      if(type == 2) {
        clearOnLcd(3);
        parkingSpot[i] = 1;
        int price = (abs((millis() - parkingTime[i])/1000) * 100);
        printOnLcd(2, String((millis() - parkingTime[i])/1000));
        printOnLcd(3, "Price: " + String(price));
        reserveSpot(1, i);
      }
      return 1;
    }  
  }
  return 0;
}

bool enterCar() {
  for(int i=0; i<9; ++i) {
    if(parkingSpot[i] == 1) {
      parkingSpot[i] = 0;
      parkingTime[i] = millis();
    }  
  }
}

void reserveSpot(int type, int index) {
  if(type == 0) {
    if(index == 0) {
      digitalWrite(LED_A1, HIGH);
    }
    if(index == 1) {
      digitalWrite(LED_A2, HIGH);
    }
    if(index == 2) {
      digitalWrite(LED_A3, HIGH);
    }
    if(index == 3) {
      digitalWrite(LED_B1, HIGH);
    }
    if(index == 4) {
      digitalWrite(LED_B2, HIGH);
    }
    if(index == 5) {
      digitalWrite(LED_B3, HIGH);
    }
    if(index == 6) {
      digitalWrite(LED_C1, HIGH);
    }
    if(index == 7) {
      digitalWrite(LED_C1, HIGH);
    }
    if(index == 8) {
      digitalWrite(LED_C3, HIGH);
    }
  } else {
    if(index == 0) {
      digitalWrite(LED_A1, LOW);
    }
    if(index == 1) {
      digitalWrite(LED_A2, LOW);
    }
    if(index == 2) {
      digitalWrite(LED_A3, LOW);
    }
    if(index == 3) {
      digitalWrite(LED_B1, LOW);
    }
    if(index == 4) {
      digitalWrite(LED_B2, LOW);
    }
    if(index == 5) {
      digitalWrite(LED_B3, LOW);
    }
    if(index == 6) {
      digitalWrite(LED_C1, LOW);
    }
    if(index == 7) {
      digitalWrite(LED_C1, LOW);
    }
    if(index == 8) {
      digitalWrite(LED_C3, LOW);
    }
  }
}
