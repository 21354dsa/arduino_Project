#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>

#define RST_PIN         D0          
#define SS_PIN          D8  //就是模組上的SDA接腳
const int button = D3;
const int led_pin = D4;
int last_btn, now_btn;
bool flag=0;

byte NEW_UID[4];

MFRC522 mfrc522;   // 建立MFRC522實體
MFRC522::MIFARE_Key key;


void LED_remind(int t=200, int n=3){
  for(int i=0; i<n; i++){
    digitalWrite(led_pin, HIGH);
    delay(t);
    digitalWrite(led_pin, LOW);
    delay(t);
  }
}

void setup() {
  pinMode(button, INPUT);
  pinMode(led_pin, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);
  //-----RC522
  SPI.begin();        // 初始化SPI介面
  mfrc522.PCD_Init(SS_PIN, RST_PIN); // 初始化MFRC522卡
  delay(4);	
  Serial.print(F("Reader "));
  Serial.print(F(": "));
  //Serial.println(mfrc522.PCD_DumpVersionToSerial());
    for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  now_btn=digitalRead(button);
  last_btn=digitalRead(button);
}

void loop() {
  // put your main code here, to run repeatedly:
  now_btn=digitalRead(button);
  if(now_btn!=last_btn){
    if(now_btn == 0) {
      flag = !flag;
      LED_remind(100,5);
    }
    last_btn = now_btn;
  }
  if(!flag) Read_RC522();
  else changeUID();
}

void Read_RC522(){
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      // 顯示卡片內容
      Serial.print(F("Card UID:"));
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size); // 顯示卡片的UID
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));  //顯示卡片的類型

      mfrc522.PICC_HaltA();  // 卡片進入停止模式
      LED_remind(250,3);
    }
}

void changeUID(){
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. And if present, select one.
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  
  // Now a card is selected. The UID and SAK is in mfrc522.uid.
  
  // Dump UID
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println();

  // Set new UID
  //byte newUid[] = NEW_UID;
  if ( mfrc522.MIFARE_SetUid(NEW_UID, (byte)4, true) ) {
    Serial.println(F("Wrote new UID to card."));
  }
  
  // Halt PICC and re-select it so DumpToSerial doesn't get confused
  mfrc522.PICC_HaltA();
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    return;
  }
  
  // Dump the new memory contents
  Serial.println(F("New UID and contents:"));
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  LED_remind(500,4);
  //delay(2000);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  //NEW_UID=buffer;
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    NEW_UID[i]=buffer[i];
    
  }
}
