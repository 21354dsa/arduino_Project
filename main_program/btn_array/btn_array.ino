#include <Keypad.h>    // 引用Keypad程式庫

#define KEY_ROWS 4 // 按鍵模組的列數
#define KEY_COLS 4 // 按鍵模組的行數

// 依照行、列排列的按鍵字元（二維陣列）
char keymap[KEY_ROWS][KEY_COLS] = {
  {'1', '2', '3', '*'},
  {'4', '5', '6', '*'},
  {'7', '8', '9', '*'},
  {'*', '0', '#', '*'}
};
/* char keymap[KEY_ROWS][KEY_COLS] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'*', '*', '*', '*'}
};//ABCD */

byte colPins[KEY_COLS] = {27, 14, 12, 13};	   // 按鍵模組，行1~4接腳。D5, D6, D7, D8
byte rowPins[KEY_ROWS] = {32, 33, 25, 26}; // 按鍵模組，列1~4接腳。 

// 初始化Keypad物件 
// 語法：Keypad(makeKeymap(按鍵字元的二維陣列), 模組列接腳, 模組行接腳, 模組列數, 模組行數)
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);

#define true_btn 4
#define false_btn 5
#define buzzer 2
int freq = 2000;//设置频率
int channel = 0;//通道号，取值0 ~ 15
int resolution = 8;//计数位数，2的8次幂=256
String Indata="";

void tone(int delay_){
  ledcWriteTone(channel, 2000);
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle = dutyCycle + 10) {
    ledcWrite(channel, dutyCycle);
    delay(1);
  }
  delay(delay_);
  ledcWrite(channel, 0);
}

void setup(){
  Serial.begin(115200);
  pinMode(true_btn, OUTPUT);
  pinMode(false_btn, OUTPUT);

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzer, channel);

  digitalWrite(true_btn, LOW);
  digitalWrite(false_btn, LOW);
}
  
void loop(){
  // 透過Keypad物件的getKey()方法讀取按鍵的字元
  
  char key = myKeypad.getKey();
  
  if (key){  // 若有按鍵被按下…
    Serial.println(key);  // 顯示按鍵的字元
    tone(200);
    switch (key) {
    case '#':
      Serial.println(Indata);
      if(Indata == "2312"){
        digitalWrite(true_btn, HIGH);
        digitalWrite(false_btn, LOW);
      }
      else{
        digitalWrite(false_btn, HIGH);
        delay(200);
        digitalWrite(false_btn, LOW);
      }
      Indata="";
      break;
    case '*':

      break;
    default :
      Indata+=key;
      break;
    
    }
  }
  delay(50);
}