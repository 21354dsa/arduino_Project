#include <FastLED.h>
#define LED_PIN     D7
#define NUM_LEDS    148
#define close D3
#define open D2
#define oled D4
int clo=1, op=1;
int setana=0;
CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(open, OUTPUT);
  pinMode(close, INPUT);
  pinMode(oled, OUTPUT);
  digitalWrite(open, 0);
  digitalWrite(oled, 1);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  
  for (int i = NUM_LEDS; i >= 0; i--) {
      leds[i] = CRGB ( 0, 0, 0);
      FastLED.show();
      delay(1);
    } 
  delay(100);
  setana = analogRead(A0);
}
void loop() {
  // Red
  
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  Serial.println("------");
  Serial.println(setana);
  if(sensorValue>setana+25 && digitalRead(close)!=0)
  {
    Serial.println("--open----");
    Serial.println(setana);
    setana=analogRead(A0);
    
    digitalWrite(oled, 0);
    for (int i = NUM_LEDS; i >= 0; i--) {
      leds[i] = CRGB ( 255, 0, 0);
      FastLED.show();
      if(digitalRead(close)==0){
        break;
      }
      delay(100);
    }
    digitalWrite(open, 1);
  }
  else if(digitalRead(close)==0 )
  {
    
    Serial.println("close");
    digitalWrite(oled, 1);
    for (int i = NUM_LEDS; i >= 0; i--) {
      leds[i] = CRGB ( 0, 0, 0);
      FastLED.show();
      delay(1);
    } 
    ESP.reset();
  }
  

}
