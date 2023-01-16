/*
該程式碼從EEPROM讀取4096位元組資料
*/
#include <EEPROM.h>

int addr = 0;

void setup() 
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start read");

  EEPROM.begin(20); //申請操作n位元組資料
  for(int i=0;i<20;i++) EEPROM.write(i, i);
  EEPROM.commit();
  for(addr = 0; addr<14; addr++)
  {
    //int data = addr*3;
    //EEPROM.write(addr, addr+2); //寫資料
    int data = EEPROM.read(addr); //讀取資料(地址)
    Serial.println(data);
  }
  //EEPROM.commit(); //儲存更改的資料

  //Serial.println("End write");
  Serial.println("End read");
}

void loop() 
{
}