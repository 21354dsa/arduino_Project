#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         D0          
#define SS_PIN          D8  //就是模組上的SDA接腳

const int button = D3;
const int led_pin = D4;
const int out_pin = D1;
const int led_out_pin = D2;

int count = 0; // 1:進入設定(長按), 2:儲存(長按)

int currentMillis, previousMillis;
bool previous_button = LOW;
String this_car[50], now_car, save_car[50];

int car_amount=0; //資料存放的地址
int newCar_count; //新增卡片的數量
int first_use=0;  //判斷不同卡片觸發不同事件

MFRC522 mfrc522;   // 建立MFRC522實體

void LED_remind(int t=200, int n=3){
  for(int i=0; i<n; i++){
    digitalWrite(led_pin, HIGH);
    delay(t);
    digitalWrite(led_pin, LOW);
    delay(t);
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(400); //申請操作2位元組資料
  car_amount = EEPROM.read(0);
  Serial.println(car_amount);
  if(car_amount > 40) car_amount = 0;
  for(int i=0; i<car_amount; i++){
    String k="";
    for(int j=0; j<8; j++){
       k += char(EEPROM.read(i*8+1+j));
    }
    this_car[i] = k;
  }
  Serial.print("memory : ");
  for(int i=0; i<car_amount; i++){
    Serial.println(this_car[i]);
  }

  pinMode(button, INPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(led_out_pin, OUTPUT);
  pinMode(out_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  digitalWrite(out_pin, LOW);
  digitalWrite(led_out_pin, LOW);

  //-----RC522
  SPI.begin();        // 初始化SPIz介面
  
  Serial.print(F("Reader "));
  Serial.print(F(": "));
  mfrc522.PCD_DumpVersionToSerial(); // 顯示讀卡設備的版本
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  mfrc522.PCD_Init(SS_PIN, RST_PIN); // 初始化MFRC522卡
  Read_Button();
  if(Read_RC522()){
    int false_ID=0;
    switch(count){
      case 0:
        //判斷是否正確
        for(int i=0; i<car_amount; i++){
          if(this_car[i].equals(now_car)){
            if(i>=1 && first_use !=0){
              digitalWrite(led_out_pin, HIGH);
              Serial.println("true_2");
              digitalWrite(out_pin, HIGH);
              LED_remind(100, 4);
              break;
              //first_use=0;
            }
            else if(i<1 && first_use == 0){
              Serial.println("true");
              digitalWrite(out_pin, HIGH);
              LED_remind(50, 4);
              digitalWrite(out_pin, LOW);
              first_use++;
            }
            
          }
          else false_ID++;
        }
        if(false_ID == car_amount) {
          digitalWrite(out_pin, LOW);
          digitalWrite(led_out_pin, LOW);
        }
        break;

      case 1:
        //設定卡號
        save_car[newCar_count] = now_car;
        newCar_count++;
        break;
    }
  }
}

int Read_RC522(){
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      // 顯示卡片內容
      Serial.print(F("Card UID:"));
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size); // 顯示卡片的UID
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));  //顯示卡片的類型

      mfrc522.PICC_HaltA();  // 卡片進入停止模式
      digitalWrite(led_pin, HIGH);
      delay(500);
      return 1;
    }
    else{
      digitalWrite(led_pin, LOW);
      return 0;
    }
}

void Read_Button(){
  bool new_button = digitalRead(button);
  if(new_button == LOW){
    if(new_button != previous_button){
      previousMillis = currentMillis;
    }
  }
  else{
    if(new_button != previous_button){
      if(currentMillis - previousMillis > 500){
        //長按3秒
        Serial.println("長按3秒放開");
        LED_remind();
        if(count == 0){
          //進入設定
          count = 1;
        }
        else if(count == 1){
          //儲存
          Save_Car();
          count = 0;
          ESP.restart();
        }
      }
    }
  }
  previous_button = new_button;
}

void Save_Car(){
  EEPROM.write(0, newCar_count);
  for(int i=0; i<newCar_count; i++){
    for(int j=0; j<8; j++){
      EEPROM.write(i*8+1+j, save_car[i][j]);
    }
  }
  EEPROM.commit();

  Serial.print("new_Car : ");
  for(int i=0; i<newCar_count; i++){
    Serial.println(save_car[i]);
  }

}

void dump_byte_array(byte *buffer, byte bufferSize) {
  now_car = "";
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    if(buffer[i] < 0x10){
      now_car += "0";
    }
    now_car += String(buffer[i], HEX);
    
  }
}

