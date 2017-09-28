/*
* Written by Mosfex : www.mosfex.com
* www.fb.com/mosfex 
* plese refer to this when using the code
*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);

const char* ssid = "mosfex.com";              //ตั้งค่า WiFi ตรงนี้นะ
const char* password = "123456789";         //ตั้งค่า password WiFi
String page_url = "mosfex";                 //ตั้งค่า panpage @account ตรงนี้นะ เช่น fb.com/mosfex <<<
const char* host = "mbasic.facebook.com";
const char* fingerprint = "93:6F:91:2B:AF:AD:21:6F:A5:15:25:6E:57:2C:DC:35:A1:45:1A:A5";

String ProcessLikes(){
  WiFiClientSecure client;
  if (!client.connect(host, 443)) { Serial.println("connection failed"); return ""; }
  client.verify(fingerprint, host);
  String url = "/"+page_url+"/community?locale=en_US";  //
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +               
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected()) {
    String line = client.readStringUntil('>');
    if(line.startsWith("<meta name=\"description\"")){      
      int st = line.indexOf(".")+2;
      int sp = line.indexOf("likes",st)-1;
      int st2 = sp + 14;
      int sp2 = line.indexOf("talking",st2)-1;
      String like = line.substring(st,sp);
      String talk = line.substring(st2,sp2);
      lcd.setCursor(4,1);
      lcd.print(like);
      lcd.write((uint8_t)6);
      lcd.write((uint8_t)7);
      /*lcd.print(" "+talk);   //disable tanking somepage may not show    
      lcd.write(0b00100011);*/
    }
  }
}
byte logo[8][8] = {
  {0x07,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},
  {0x1F,0x1F,0x1E,0x1C,0x18,0x18,0x18,0x10},
  {0x1C,0x1E,0x03,0x03,0x03,0x1F,0x1F,0x07},
  {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x07},
  {0x10,0x10,0x18,0x18,0x18,0x18,0x18,0x18},
  {0x07,0x07,0x1F,0x1F,0x1F,0x1F,0x1E,0x1C},
  {0x00,0x00,0x00,0x0e,0x0d,0x0d,0x0d,0x0e},
  {0x18,0x14,0x14,0x16,0x01,0x01,0x11,0x1e}
};

void PrintBackground()
{
  for(int i=0;i<=7;i++)
    lcd.createChar(i, logo[i]);  
  lcd.setCursor(0,0);
  lcd.write((uint8_t)0);
  lcd.write((uint8_t)1);
  lcd.write((uint8_t)2);
  lcd.setCursor(0,1);
  lcd.write((uint8_t)3);
  lcd.write((uint8_t)4);
  lcd.write((uint8_t)5);
  lcd.setCursor(3,0);
  lcd.print("fb.com/mosfex");  
}

void setup() {
  Serial.begin(115200);  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }     
  lcd.init();
  lcd.backlight();
  PrintBackground();
}

void loop() {
  ProcessLikes();  
  delay(1000);
}

