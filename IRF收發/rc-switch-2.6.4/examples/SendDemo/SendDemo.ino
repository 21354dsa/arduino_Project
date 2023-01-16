/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
int now=0;
void setup() {
  now=0;
  Serial.begin(9600);
  pinMode(12,OUTPUT);
  // Transmitter is connected to Arduino Pin #10  
  pinMode(16,INPUT);
  pinMode(14,INPUT_PULLUP);
  mySwitch.enableTransmit(12);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
   //mySwitch.setPulseLength(500);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}
int count=0;
void loop() {
  Serial.println(digitalRead(14));

  if(now!=digitalRead(14)){
    mySwitch.send(2992836, 24);
    count=0;
    delay(1000);
  };
  now=int(digitalRead(14));
  if(count<10){
    if(now==1){
      mySwitch.send(2992833, 24);//down
    }else{
      mySwitch.send(2992840, 24);//up
    }
    count++;
  }
}
