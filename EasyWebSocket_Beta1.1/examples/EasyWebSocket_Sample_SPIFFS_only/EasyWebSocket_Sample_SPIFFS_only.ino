/*
This is a sample sketch to control dimming the LED in the smartphone browser using 
WebSocket for ESP-WROOM-02 ( ESP8266 )
Beta version 1.1
---> https://www.mgo-tec.com

Please rewrite their own the ssid and password.
Please rewrite their own local IP address of "/data/spiffs_only.html" in the sketch folder.

The MIT License (MIT)

Copyright (c) 2015 mgo-tec

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <EasyWebSocket.h>
#include <ESP8266WiFi.h>
#include <Hash.h>

const char* ssid = "xxxx";
const char* password = "xxxx";

long CountTime;

byte cnt = 0;

EasyWebSocket ews;

String ret_str;

int PingSendTime = 3000;

#define ledPin1 12
#define ledPin2 13
#define ledPin3 14

void setup() 
{  
  ews.AP_Connect(ssid, password);

  ews.EWS_HandShake_SPIFFS("spiffs_only.html");//File names in the data folder in the sketch folder
  
  CountTime = millis();
}

void loop() {
  String str;
  if(ret_str != "_close"){
    
    if(millis()-CountTime > 300){//Data transmission from WROOM (ESP8266) every 300ms
      if(cnt > 5){
        cnt = 0;
      }
      switch(cnt){
        case 0:
          str = "WebSocket";
          break;
        case 1:
          str = "ESP-WROOM";
          break;
        case 2:
          str = "8X8 DotLED";
          break;
        case 3:
          str = "Controll";
          break;
        case 4:
          str = "Hello!!";
          break;
        case 5:
          str = "World!!";
          break;
      }
      
      ews.EWS_ESP8266_Str_SEND(str, "text");
      CountTime = millis();
      cnt++;
    }

    ret_str = ews.EWS_ESP8266CharReceive(PingSendTime);
    if(ret_str != "\0"){
      Serial.println(ret_str);
      if(ret_str != "Ping"){
        int ws_data = (ret_str[0]-0x30)*100 + (ret_str[1]-0x30)*10 + (ret_str[2]-0x30);
        switch(ret_str[4]){
          case 'B':
            LED_PWM('S', ledPin1, floor(ws_data/2));
            break;
          case 'G':
            LED_PWM('S', ledPin2, floor(ws_data/2));
            break;
          case 'R':
            LED_PWM('S', ledPin3, floor(ws_data/2));
            break;
          case '_':
            LED_PWM('A', 0, floor(ws_data/2));
            break;
          case 'b':
            LED_PWM('S', ledPin1, ws_data);
            break;
          case 'g':
            LED_PWM('S', ledPin2, ws_data);
            break;
          case 'r':
            LED_PWM('S', ledPin3, ws_data);
            break;
          case '-':
            LED_PWM('A', 0, ws_data);
            break;
        }
      }
    }
  }else if(ret_str == "_close"){
    delay(100);
    ews.EWS_HandShake_SPIFFS("spiffs_only.html");//File names in the data folder in the sketch folder
    CountTime = millis();
    ret_str = "";
  }
}

//**************************************************************
void LED_PWM(char S_or_A, byte Led_Pin, int data_i)
{
Serial.println(data_i);
  switch(S_or_A){
    case 'S':
      analogWrite(Led_Pin, data_i);
      break;
    case 'A':
      if(data_i < 34){
        analogWrite(ledPin1, data_i*8);
        analogWrite(ledPin2, 0);
        analogWrite(ledPin3, 0);
      }else if(data_i > 33 && data_i < 67){
        analogWrite(ledPin2, (data_i-33)*8);
        analogWrite(ledPin1, 0);
        analogWrite(ledPin3, 0);
      }else if(data_i > 66){
        analogWrite(ledPin3, (data_i-66)*7.78);
        analogWrite(ledPin1, 0);
        analogWrite(ledPin2, 0);
      }
      break;
  }
}
