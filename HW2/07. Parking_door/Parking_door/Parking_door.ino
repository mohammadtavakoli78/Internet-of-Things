#include <Keypad.h>

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

void setup() {
  Serial.begin(9600);
}

void loop() {
  char inputKey = keypad.getKey();

  if (inputKey) {
    if (inputKey == '1') {
      Serial.print("1");
    } else if (inputKey == '3'){
      Serial.print("3");
    }
  }
}
