#include "TRIGGER_WIFI.h"               /*Includes ESP8266WiFi.h and WiFiClientSecure.h, just have these two libraries downloaded before*/
#include "TRIGGER_GOOGLESHEETS.h" 
#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
SoftwareSerial mySerial(4,5);
const char* wifiName= "No Internet";
const char* wifiPass= "ahmadmunib13";/*Library file for Google Sheets, has to be used after Wi-Fi Client Secure declaration, here everything is in Trigger_WIFI.h, so using it after Trigger_WIFI.h*/ 

/**********Google Sheets Definations***********/
char column_name_in_sheets[ ][20] = {"value"};                        /*1. The Total no of column depends on how many value you have created in Script of Sheets;2. It has to be in order as per the rows decided in google sheets*/
String Sheets_GAS_ID = "AKfycbyG2dfIWMeF_rXpkZNFRG8TyDtZNlnGGi-j2h_HjZty8BYszchi";                                         /*This is the Sheets GAS ID, you need to look for your sheets id*/
int No_of_Parameters = 1;                                                                /*Here No_of_Parameters decides how many parameters you want to send it to Google Sheets at once, change it according to your needs*/
/*********************************************/

void setup() 
{
  Serial.begin(9600);
mySerial.begin(4800);
while (!Serial) continue;
  Serial.print("Connecting to ");
  Serial.println(wifiName);
  WiFi.begin(wifiName, wifiPass);                                                /*Provide you Wi-Fi SSID and password to connect to Wi-Fi*/
  Google_Sheets_Init(column_name_in_sheets, Sheets_GAS_ID, No_of_Parameters );         /*Sets the column name for Google Sheets, the GAS ID, and the No of Parameter we want to send*/        
}

void loop() 
{
    const size_t capacity = JSON_OBJECT_SIZE(4) + 20;
DynamicJsonDocument doc(capacity);
DeserializationError error = deserializeJson(doc, mySerial);
if (error) {
Serial.print(F("deserializeJson() failed: "));
Serial.println(error.c_str());
return;
}
int data1 = doc["data1"]; // 100
int data2 = doc["data2"];
//int data3 = doc["data3"];
//serializeJson(doc, Serial);

JsonObject object = doc.as<JsonObject>();
const int a = object["data2"];
Serial.println(a);                        /*Demo values that has to be sent to google sheets, you can use sensor values*/

  Data_to_Sheets(No_of_Parameters,a);         /*1. This function accepts multiple float parameter, here No_of_Parameters decides how many parameters you want to send to Google Sheets; 2. The values sent should be in order as per the column in Google Sheets*/

  Serial.println();
  delay(10000);                                       /*10 Sec Delay, Here 10 second delay is just used so that we can see the data pushed to sheets one by one
                                                        There is nothing like a minimum delay between two data push*/
}
