/*
This is a sample sketch to control dimming the LED in the smartphone browser using 
WebSocket for ESP-WROOM-02 ( ESP8266 )
Beta version 1.1
---> https://www.mgo-tec.com

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

String html_str1;
String html_str2;
String html_str3;
String html_str4;
String html_str5;
String html_str6;
String html_str7;

String ret_str;

int PingSendTime = 3000;

#define ledPin1 12
#define ledPin2 13
#define ledPin3 14

void setup() 
{
  //The assignment in the Body element of the browser of HTML to a String variable
  html_str1 = "<body text='white' bgcolor='black'>\r\n";
  html_str1 += "<font size=3>\r\n";
  html_str1 += "ESP-WROOM-02(ESP8266)\r\n";
  html_str1 += "<br>\r\n";
  html_str1 += "WebSocket Test\r\n";
  html_str1 += "</font><br>\r\n";
  html_str1 += ews.EWS_BrowserSendRate();
  html_str1 += "<br>\r\n";

  html_str1 += "from WROOM(ESP8266) DATA =\r\n";
  html_str1 += ews.EWS_BrowserReceiveTextTag("text",20,"RED");
  html_str1 += "<br>\r\n";
  html_str1 += "WS Status = \r\n";
  html_str1 += ews.EWS_Status_Text(20,"#FF00FF");
  html_str1 += "<br>\r\n";
  
  html_str2 = "<br>LED BLUE... Dim\r\n";
  html_str2 += ews.EWS_Canvas_Slider_T("BLUE",200,40,"#777777","#0000ff");
  html_str2 += "<br>LED GREEN Dim\r\n";
  html_str2 += ews.EWS_Canvas_Slider_T("GREEN",200,40,"#777777","#00ff00");
  html_str3 = "<br>LED RED..... Dim\r\n";
  html_str3 += ews.EWS_Canvas_Slider_T("RED",200,40,"#777777","#ff0000");
  html_str3 += "<br>LED RGB..... Dim\r\n";
  html_str3 += ews.EWS_Canvas_Slider_T("_RGB",200,40,"#777777","#ffff00");
  
  html_str4 = "<br><br>\r\n";
  html_str4 += "BLUE... \r\n";
  html_str4 += ews.EWS_OnOff_Button("blue",60,25,15,"#FFFFFF","#0000FF");
  html_str4 += ews.EWS_Touch_Slider_T("blue", "Txt1");
  html_str4 += ews.EWS_Sl_Text("Txt1",15,"#5555FF");
  html_str4 += "<br><br>\r\n";
  html_str4 += "GREEN \r\n";
  html_str4 += ews.EWS_OnOff_Button("green",60,25,15,"#FFFFFF","#00FF00");
  html_str4 += ews.EWS_Touch_Slider_T("green", "Txt2");
  html_str4 += ews.EWS_Sl_Text("Txt2",15,"#00ff00");
  html_str4 += "<br><br\r\n>";
  html_str5 = "RED..... \r\n";
  html_str5 += ews.EWS_OnOff_Button("red",60,25,15,"#FFFFFF","#ff0000");
  html_str5 += ews.EWS_Touch_Slider_T("red", "Txt3");
  html_str5 += ews.EWS_Sl_Text("Txt3",15,"#ff0000");
  html_str5 += "<br><br>\r\n";
  html_str5 += "RGB..... \r\n";
  html_str5 += ews.EWS_Touch_Slider_BT("-RGB", "Txt4");
  html_str5 += ews.EWS_Sl_BoxText("Txt4",30,20,15,"#000000");
  html_str5 += "<br><br><br>\r\n";  
  html_str5 += ews.EWS_Close_Button("WS CLOSE",150,40,17);
  html_str5 += "</body></html>\r\n";

  html_str6 = ""; //The description here is if the string is too large. If you do not want to use it is empty.
  html_str7 = ""; //The description here is if the string is too large. If you do not want to use it is empty.
  
  ews.AP_Connect(ssid, password);

  ews.EWS_HandShake(html_str1, html_str2, html_str3, html_str4, html_str5, html_str6, html_str7);
  
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
    ews.EWS_HandShake(html_str1, html_str2, html_str3, html_str4, html_str5, html_str6, html_str7);
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
