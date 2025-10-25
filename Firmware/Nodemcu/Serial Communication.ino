#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include<SoftwareSerial.h>
SoftwareSerial mySerial(4,5);


const char* ssid = "No Internet";    //your wifi network
const char* password = "munibnishat6";     //yout wifi pasword
const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client;

const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycbyG2dfIWMeF_rXpkZNFRG8TyDtZNlnGGi-j2h_HjZty8BYszchi";     //spreadsheet id

void setup() 
{
  
 Serial.begin(9600);
mySerial.begin(4800); 
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setInsecure();
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
int data3 = doc["data3"];
//serializeJson(doc, Serial);

JsonObject object = doc.as<JsonObject>();
const int value = object["data2"];
const int value1 = object["data1"];
const int value2 = object["data3"];
Serial.println(value);
Serial.println(value1);
Serial.println(value2);
  sendData(value,value1,value2);
  Serial.println();
  delay(1000);
}

void sendData(int value,int value1,int value2)
{
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
  Serial.println("certificate matches");
  } else {
  Serial.println("certificate doesn't match");
  }
  String url = "/macros/s/" + GAS_ID + "/exec?Current_Balance="+value +"&TakaIn="+value1 +"&TakaOut="+value2;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
    Serial.println("headers received");
    break;
  }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
  } else {
  Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
} 
