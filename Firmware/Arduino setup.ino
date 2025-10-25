#include <ArduinoJson.h>
#include "NewPing.h"
#define S0 3
#define S1 4
#define S2 5
#define S3 6
#define sensorOut 8
#define TRIGGER_PIN_1  A3
#define ECHO_PIN_1     A0
#define TRIGGER_PIN_2  A8
#define ECHO_PIN_2     A9
#define MAX_DISTANCE 400
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
float duration1,duration2,distance1,distance2;
int iterations = 5,frequencyR=0, frequencyG=0, frequencyB =0,G_R,B_R,G_B,G_B_R,sonar,color,sum=0;
void setup() {
Serial2.begin(4800);
  lcd.begin();
  lcd.backlight();
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
}
 
void loop() {
  lcd.clear();
  getFreq();
  sonar=sonarData();
  color=showColor();
  if(sonar==0){
  const size_t capacity = JSON_OBJECT_SIZE(4);
DynamicJsonDocument doc(capacity);
sum=sum+color;
doc["data1"] = color;
doc["data2"] = sum;
doc["data3"]=0;
serializeJson(doc, Serial2);
  lcd.setCursor(0,0);
  lcd.print(color);
  lcd.print(" Taka IN");
  lcd.setCursor(0,1);
  lcd.print("Balance:");
  //Serial.print(sum=sum+color);
  lcd.print(sum);
  }
  if(sonar==1){
  const size_t capacity = JSON_OBJECT_SIZE(4);
DynamicJsonDocument doc(capacity);
sum=sum-color;
doc["data3"] = color;
doc["data2"] = sum;
doc["data1"]=0;
serializeJson(doc, Serial2);
  lcd.setCursor(0,0);
  lcd.print(color);
  lcd.print(" Taka 0ut");
  lcd.setCursor(0,1);
  //Serial.print(sum=sum-color);
  lcd.print("Balance:");
  lcd.print(sum);}
  
  if(sonar==3){
  const size_t capacity = JSON_OBJECT_SIZE(4);
DynamicJsonDocument doc(capacity);
sum=sum+0;
doc["data1"] = 0;
doc["data2"] = sum;
doc["data3"]=0;
serializeJson(doc, Serial2);
  lcd.setCursor(0,1);
  //Serial.print(sum=sum+0);
  lcd.print("Balance:");
  lcd.print(sum);}
delay(5000);
}
void getFreq(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  frequencyR = pulseIn(sensorOut, LOW);
  frequencyR = map(frequencyR, 20,44,0,255);
 
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  frequencyG = pulseIn(sensorOut, LOW);
  frequencyG = map(frequencyG,24,44,0,255);
  
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  frequencyB = pulseIn(sensorOut, LOW);
  frequencyB = map(frequencyB, 17,46,0,255);
  G_R=frequencyG-frequencyR;
  B_R=frequencyB-frequencyR;
  G_B=frequencyG-frequencyB;
  G_B_R=frequencyG-frequencyB-frequencyR;
}
int showColor()
{
  if(frequencyR>frequencyB){
    if(frequencyR>720){
      return 100;
    }
    else{ 
    if(G_R>200){
      return 1000;
    }
    else {
      return 500;}}
    }
  else if(frequencyR<frequencyB)
{
  if(G_R>300){
    if(G_B>200&&G_B_R>-200){ 
      return 10;
    }
    else if(G_B>200&&G_B_R<-200){
      return 50;
    }}
  else{
      if(frequencyB>500){
      return 5;}
   else
   {return 20;}}}
  else if(frequencyG>2000&&frequencyR>2000){
  return 0;
  }}
int sonarData(){
  duration1 = sonar1.ping_median(iterations);
  duration2 = sonar2.ping_median(iterations);
  distance1 = (duration1 / 2) *0.034;
  distance2 = (duration2 / 2) *0.034;
    if (distance1 <= 20&&distance1>=0){
    return 0;}
    if (distance2 <= 20&&distance1>=0){
    return 1;}
    if(distance1>20&&distance2>20)
    return 3;
    }
