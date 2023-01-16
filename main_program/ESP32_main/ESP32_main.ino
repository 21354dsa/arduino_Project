#include <EEPROM.h>
const int inIO[] = { 16, 17, 18};//, 19, 22, 23, 25, 26};//, 34, 35, 36
const int tch[]={};//12,13,14,15,27,32,33};
int IOlen = sizeof(inIO) / 4;
int th_len=sizeof(tch)/4;
int addr = 0;
byte High_len;
int out_pin = 4;
int rst_pin = 2;

void setup() {
  EEPROM.begin(200);
  Serial.begin(9600);
  Serial.println(EEPROM.read(addr));
  for (int i = 0; i < IOlen; i++) {
    pinMode(inIO[i], INPUT_PULLDOWN);
  }
  for (int i = 0; i < th_len; i++) {
    pinMode(tch[i], INPUT_PULLDOWN);
  }
  pinMode(out_pin, OUTPUT);
  pinMode(rst_pin, INPUT);
  digitalWrite(out_pin, LOW);
}//18 19 22 23 17 15 


void loop() {
  int all_HIGH = 0;
  High_len = EEPROM.read(addr);
  for (int i = 0; i < IOlen; i++) {

    if (digitalRead(inIO[i]) == HIGH) {
     Serial.printf("%d, ",inIO[i]);
        all_HIGH++;

    }else{
        delay(70);//依照狀況調整
         if (digitalRead(inIO[i]) == HIGH) {
          Serial.printf("%d, ",inIO[i]);
          all_HIGH++;
         }
    }
    
  }
  //Serial.println();
  for (int i = 0; i < th_len; i++) {
    int tch_v=touchRead(tch[i]);
        
    if (tch_v==1) {
        Serial.printf("%d, ",tch[i]);
        all_HIGH++;
    } else{
      delay(70);
      if(digitalRead(tch[i])==HIGH){
        Serial.printf("%d, ",tch[i]);
        all_HIGH++;
      }
    }
  }
  //Serial.println();  
  Serial.println(all_HIGH);
  if (digitalRead(rst_pin) == HIGH) {
    EEPROM.write(addr, all_HIGH);
    High_len = EEPROM.read(addr);
    Serial.println("updata");
    
  }

  if (all_HIGH >= High_len) {
    digitalWrite(out_pin, HIGH);
  } else {
    digitalWrite(out_pin, LOW);
  }
  EEPROM.commit();

}