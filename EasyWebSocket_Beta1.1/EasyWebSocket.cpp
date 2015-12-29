/*
  EasyWebsocket.cpp - WebSocket for ESP-WROOM-02 ( esp8266 )
  Beta version 1.1

The MIT License (MIT)

Copyright (c) 2015 Mgo-tec

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

#include "Arduino.h"
#include "EasyWebsocket.h"

const char* GUID_str = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

WiFiClient client;
WiFiServer server(80);

EasyWebSocket::EasyWebSocket(){}

//********AP(Router) Connection****
void EasyWebSocket::AP_Connect(const char* ssid, const char* password)
{
  Serial.begin(115200);
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  delay(10);
}

//********WebSocket Hand Shake ****************
void EasyWebSocket::EWS_HandShake(String _res_html1, String _res_html2, String _res_html3, String _res_html4, String _res_html5, String _res_html6, String _res_html7)
{
  client = server.available();
  delay(1);
  String req;
  String hash_req_key;
  
  while(!_WS_on){
    delay(1);
    switch(_Ini_html_on){
    case false:
      client = server.available();
      while(client){
        if(client.available()){
          req = client.readStringUntil('\n');
          Serial.println(req);
          if (req.indexOf("GET / HTTP") != -1){
            Serial.println("-----from Browser FirstTime HTTP Request---------");
            Serial.println(req);

            while(req.indexOf("\r") != 0){
              req = client.readStringUntil('\n');

              if(req.indexOf("Android") != -1){
                _Android_or_iPad = 'A';
              }else if(req.indexOf("iPad") != -1){
                _Android_or_iPad = 'i';
              }else if(req.indexOf("iPhone") != -1){
                _Android_or_iPad = 'P';
              }
              Serial.println(req);
            }
            req = "";
            delay(10);

            client.print(F("HTTP/1.1 200 OK\r\n"));
            client.print(F("Content-Type:text/html\r\n"));
            client.print(F("Connection:close\r\n\r\n"));
            
            SPIFFS.begin();
            
            File f1 = SPIFFS.open("/spiffs_01.txt", "r");
            char c = f1.read();
            client.print(c);
            while(c<0xDE){
              c= f1.read();
              if(c>0xDD) break;
              client.print(c);
            }

            client.print(_res_html1);
            client.print(_res_html2);
            client.print(_res_html3);
            client.print(_res_html4);
            client.print(_res_html5);
            client.print(_res_html6);
            client.print(_res_html7);
            
            _res_html1 = "";
            _res_html2 = "";
            _res_html3 = "";
            _res_html4 = "";
            _res_html5 = "";
            _res_html6 = "";
            _res_html7 = "";
            
            delay(1);
            client.stop();

            Serial.println("\nGET HTTP client stop--------------------");
            req = "";
            _Ini_html_on = true;

            if(_Android_or_iPad == 'i'){
              break;
            }
          }
        }
      }
      break;
    case true:
      switch(_WS_on){
        case false:
          EasyWebSocket::EWS_HTTP_Responce();
          _PingLastTime = millis();
          _PongLastTime = millis();
          
          break;
        case true:
          Serial.println("WS_Conplete!!!!!!!");
          break;
      }
      break;
    }
  }
}

//********WebSocket Hand Shake ****************
void EasyWebSocket::EWS_HandShake_SPIFFS(String _spiffs)
{
  client = server.available();
  delay(1);
  String req;
  String hash_req_key;
  
  while(!_WS_on){
    delay(1);
    switch(_Ini_html_on){
    case false:
      client = server.available();
      while(client){
        if(client.available()){
          req = client.readStringUntil('\n');
          Serial.println(req);
          if (req.indexOf("GET / HTTP") != -1){
            Serial.println("-----from Browser FirstTime HTTP Request---------");
            Serial.println(req);

            while(req.indexOf("\r") != 0){
              req = client.readStringUntil('\n');

              if(req.indexOf("Android") != -1){
                _Android_or_iPad = 'A';
              }else if(req.indexOf("iPad") != -1){
                _Android_or_iPad = 'i';
              }else if(req.indexOf("iPhone") != -1){
                _Android_or_iPad = 'P';
              }
              Serial.println(req);
            }
            req = "";
            delay(10);

            client.print(F("HTTP/1.1 200 OK\r\n"));
            client.print(F("Content-Type:text/html\r\n"));
            client.print(F("Connection:close\r\n\r\n"));
            
            SPIFFS.begin();
            
            File f1 = SPIFFS.open(_spiffs, "r");
            char c = f1.read();
            client.print(c);
            while(c<0xDE){
              c= f1.read();
              if(c>0xDD) break;
              client.print(c);
            }
            
            _spiffs = "";
            
            delay(1);
            client.stop();

            Serial.println("\nGET HTTP client stop--------------------");
            req = "";
            _Ini_html_on = true;

            if(_Android_or_iPad == 'i'){
              break;
            }
          }
        }
      }
      break;
    case true:
      switch(_WS_on){
        case false:
          EasyWebSocket::EWS_HTTP_Responce();
          _PingLastTime = millis();
          _PongLastTime = millis();
          
          break;
        case true:
          Serial.println("WS_Conplete!!!!!!!");
          break;
      }
      break;
    }
  }
}

//************HTTP Response**************************
void EasyWebSocket::EWS_HTTP_Responce()
{
  client = server.available();
  delay(1);
  String req;
  String hash_req_key;
  
  while(client){
    if(client.available()){
      req = client.readStringUntil('\n');
      Serial.println(req);
      if (req.indexOf("websocket") != -1){
        Serial.println("-----from Browser HTTP WebSocket Request---------");
        Serial.println(req);
 
        while(req.indexOf("\r") != 0){
          req = client.readStringUntil('\n');
          Serial.println(req);
          if(req.indexOf("Sec-WebSocket-Key")>=0){
            hash_req_key = req.substring(req.indexOf(':')+2,req.indexOf('\r'));
            Serial.println();
            Serial.print("hash_req_key =");
            Serial.println(hash_req_key);
          }        
        }
  
        delay(10);
        req ="";
  
        char h_resp_key[29];

        EasyWebSocket::Hash_Key(hash_req_key, h_resp_key);
        
        Serial.print("h_resp_key = ");
        Serial.println(h_resp_key);
        String str;

        str = "HTTP/1.1 101 Switching Protocols\r\n";
        str += "Upgrade: websocket\r\n";
        str += "Connection: Upgrade\r\n";
        str += "Sec-WebSocket-Accept: ";
        for(byte i=0; i<28; i++){
          str += h_resp_key[i];
        }

        str += "\r\n\r\n";
        
        Serial.println("-----HTTP Respons start-------");
        Serial.println(str);
        client.print(str);
        str = "";
  
        _WS_on = true;
        break;
  
      }else if(req.indexOf("favicon") != -1){

        Serial.println();
        Serial.println("--****GET favicon Request************");
        Serial.print(req);
        while(client.available()){

          Serial.write(client.read());
        }
        delay(1);
        client.stop();
        delay(1);
        Serial.println();
        Serial.println("favicon_Client Stop--------------");
      }
    }
  }
}

//************Hash sha1 base64 encord**************************
void EasyWebSocket::Hash_Key(String h_req_key, char* h_resp_key)
{
  char Base64[65] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
                      'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
                      'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
                      'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/',
                      '='
                    };
  byte hash_six[27];
  byte dummy_h1, dummy_h2;
  byte bb;
  byte i, j;
  i=0;
  j=0;
  
  String merge_str;

  merge_str = h_req_key + String(GUID_str);
  Serial.println();
  Serial.print("merge_str =");
  Serial.println(merge_str);
  Serial.print("SHA1:");
  Serial.println(sha1(merge_str));

  byte hash[20];
  sha1(merge_str, &hash[0]);

  for( i = 0; i < 20; i++) {
    hash_six[j] = hash[i]>>2;
    
    hash_six[j+1] = hash[i+1] >> 4;
    bitWrite(hash_six[j+1], 4, bitRead(hash[i],0));
    bitWrite(hash_six[j+1], 5, bitRead(hash[i],1));
    
    if(j+2 < 26){
      hash_six[j+2] = hash[i+2] >> 6;
      bitWrite(hash_six[j+2], 2, bitRead(hash[i+1],0));
      bitWrite(hash_six[j+2], 3, bitRead(hash[i+1],1));
      bitWrite(hash_six[j+2], 4, bitRead(hash[i+1],2));
      bitWrite(hash_six[j+2], 5, bitRead(hash[i+1],3));
    }else if(j+2 == 26){
      dummy_h1 = 0;
      dummy_h2 = 0;
      dummy_h2 = hash[i+1] << 4;
      dummy_h2 = dummy_h2 >>2;
      hash_six[j+2] = dummy_h1 | dummy_h2;
    }
    
    if( j+3 < 27 ){
      hash_six[j+3] = hash[i+2];
      bitWrite(hash_six[j+3], 6, 0);
      bitWrite(hash_six[j+3], 7, 0);
    }else if(j+3 == 27){
      hash_six[j+3] = '=';
    }
    
    h_resp_key[j] = Base64[hash_six[j]];
    h_resp_key[j+1] = Base64[hash_six[j+1]];
    h_resp_key[j+2] = Base64[hash_six[j+2]];
    
    if(j+3==27){
      h_resp_key[j+3] = Base64[64];
      break;
    }else{
      h_resp_key[j+3] = Base64[hash_six[j+3]];
    }
    
    i = i + 2;
    j = j + 4;
  }
  h_resp_key[28] = '\0';

  Serial.print("hash_six = ");
  for(i=0; i<28; i++){
    Serial.print(hash_six[i],BIN);
    Serial.print('_');
  }
  Serial.println();
}
  
void EasyWebSocket::EWS_ESP8266_Str_SEND(String str, String id)
{
  str += '|' + id + '|' +'\0';
  client.write(B10000001);
  client.write(str.length());
  client.print(str); 
}

String EasyWebSocket::EWS_ESP8266CharReceive(int pTime)
{
  byte b=0;
  byte data_len;
  byte mask[4];
  byte i;
  String str_close = "_close";

  if(pTime > 0){
    if(millis()-_PingLastTime > pTime){
      EasyWebSocket::EWS_PING_SEND();
      _PingLastTime = millis();
    }

    if((millis() - _PongLastTime) > (pTime + 500)){
      delay(1);
      client.stop();
      delay(1);
      Serial.println();
      Serial.println("Pong_Client.STOP-----------------");
      _WS_on = false;
      _Ini_html_on = false;
      return str_close;
    }
  }
 
  if(client.available()){
    b = client.read();
    if(b == B10000001 || b == B10001010){
      switch (b){
        case B10000001:
          _PingLastTime = millis();
          _PongLastTime = millis();
          break;
        case B10001010:
          _PongLastTime = millis();
          Serial.println("Pong Receive**********");
          break;
      }

      b = client.read();
      data_len = b - B10000000;

      for(i=0; i<4; i++){
        mask[i] = client.read();
      }
      
      byte m_data[data_len];
      char data_c[data_len + 1];
          
      for(i = 0; i<data_len; i++){
        m_data[i] = client.read();
        data_c[i] = mask[i%4]^m_data[i];
      }
      data_c[data_len] = '\0';

      return String(data_c);

    }else if(b == B10001000){
      Serial.println("Close Receive------------");
      delay(1);
      client.write(B10001000);
      delay(1);
      Serial.println("Close Send------------");
      Serial.println(b,BIN);
      return str_close;
    }
  }else{
    return String('\0');
  }
}

void EasyWebSocket::EWS_PING_SEND()
{
  client.write(B10001001);
  client.write(4);
  client.print("Ping");
  Serial.println();
  Serial.println("Ping Send-----------");
}

String EasyWebSocket::EWS_Body_style(String text_color, String bg_color)
{
  String str;
  str = "<body text='";
  str += text_color;
  str += "' bgcolor='";
  str += bg_color;
  str += "'>\r\n";
  return str;
}

String EasyWebSocket::EWS_OnOff_Button(String button_id, int width, int height, byte font_size, String f_color, String b_color)
{
  String str;
  str = "<input type='button' value='OFF' onClick=\"OnOffBt(this,'";
  str += button_id;
  str += "')\"";
  str += " style='width:";
  str += String(width);
  str += "px; ";
  str += "height:";
  str += String(height);
  str += "px; font-size:";
  str += String(font_size);
  str += "px; color:";
  str += f_color;
  str += "; background-color:";
  str += b_color;
  str += ";' >\r\n";
  
  return str;
}

String EasyWebSocket::EWS_Touch_Slider_BT(String slider_id, String vbox_id)
{
  String str;
  str += "<input type='range' ontouchmove=\"doSend(this.value,'";
  str += slider_id;
  str += "'); document.getElementById('";
  str += vbox_id;
  str += "').value=this.value;\">\r\n";
  return str;
}

String EasyWebSocket::EWS_Touch_Slider_T(String slider_id, String txt_id)
{
  String str;
  str += "<input type='range' ontouchmove=\"doSend(this.value,'";
  str += slider_id;
  str += "'); document.getElementById('";
  str += txt_id;
  str += "').innerHTML=this.value;\">\r\n";
  return str;
}


String EasyWebSocket::EWS_Mouse_Slider_BT(String slider_id, String vbox_id)
{
  String str;
  str += "<input type='range' onMousemove=\"doSend(this.value,'";
  str += slider_id;
  str += "'); document.getElementById('";
  str += vbox_id;
  str += "').value=this.value;\">\r\n";
  return str;
}

String EasyWebSocket::EWS_Mouse_Slider_T(String slider_id, String txt_id)
{
  String str;
  str += "<input type='range' onMousemove=\"doSend(this.value,'";
  str += slider_id;
  str += "'); document.getElementById('";
  str += txt_id;
  str += "').innerHTML=this.value;\">\r\n";
  return str;
}

String EasyWebSocket::EWS_Sl_BoxText(String vbox_id, int width, int height, byte font_size, String color)
{
  String str;
  str = "<input type='number' id='";
  str += vbox_id;
  str += "' style='width:";
  str += String(width);
  str += "px; ";
  str += "height:";
  str += String(height);
  str += "px; font-size:";
  str += String(font_size);
  str += "px; color:";
  str += String(color);
  str += ";' >\r\n";
  return str;
}

String EasyWebSocket::EWS_Sl_Text(String text_id, byte font_size, String color)
{
  String str;
  str = "<span id='";
  str += text_id;
  str += "' style='font-size:";
  str += String(font_size);
  str += "px; color:";
  str += String(color);
  str += ";' ></span>\r\n";
  return str;
}

String EasyWebSocket::EWS_BrowserReceiveTextTag(String id, byte font_size, String fnt_col)
{
  String str;
  str = "<span id='" + id;
  str += "' style='font-size:";
  str += String(font_size);
  str += "px; color:" + fnt_col + ";'></span>\r\n";
  return str;
}

String EasyWebSocket::EWS_Close_Button(String name, int width, int height, byte font_size)
{
  String str;
  str = "<input type='button' value='";
  str += name;
  str += "' style='width:";
  str += String(width);
  str += "px; ";
  str += "height:";
  str += String(height);
  str += "px; font-size:";
  str += String(font_size);
  str += "px;' onclick='WS_close()'>\r\n";
  return str;
}

String EasyWebSocket::EWS_BrowserSendRate()
{
  String str;
  str += "<form name='fRate'>\r\n";
  str += "  <select id='selRate'>\r\n";
  str += "    <option value=0>0ms</option>\r\n";
  str += "    <option value=5>5ms</option>\r\n";
  str += "    <option value=10>10ms</option>\r\n";
  str += "    <option value=15>15ms</option>\r\n";
  str += "    <option value=20>20ms</option>\r\n";
  str += "    <option value=25>25ms</option>\r\n";
  str += "    <option value=30>30ms</option>\r\n";
  str += "    <option value=35>35ms</option>\r\n";
  str += "    <option value=40>40ms</option>\r\n";
  str += "    <option value=45>45ms</option>\r\n";
  str += "    <option value=50>50ms</option>\r\n";
  str += "  </select>\r\n";
  str += "  <input type='button' value='Rate Exec' onclick='onButtonRate();' />\r\n";
  str += "  Transfer Rate= <span id='RateTxt'>0</span>ms\r\n";
  str += "</form>\r\n";
  return str;
}

String EasyWebSocket::EWS_Status_Text(byte font_size, String color)
{
  String str;
  str = "<span id='__wsStatus__' style='font-size:";
  str += String(font_size);
  str += "px; color:";
  str += String(color);
  str += ";' ></span>\r\n";
  return str;
}

String EasyWebSocket::EWS_Canvas_Slider_T(String slider_id, int width, int height, String frame_col, String fill_col)
{
  String str;
  str = "<canvas id='" + slider_id + "' width='" + String(width) + "' height='" + String(height) + "'></canvas>\r\n";
  
  str += "<script type='text/javascript'>\r\n";
  str += "  fnc_" + slider_id + "();\r\n";
  str += "  function fnc_" + slider_id + "(){\r\n";
  str += "    var c_w = " + String(width) + ";\r\n";
  str += "    var c_h = " + String(height) + ";\r\n";
  str += "    var line_width = 5;\r\n";
  str += "    var canvas2 = document.getElementById('" + slider_id + "');\r\n";
  str += "    var ctx2 = canvas2.getContext('2d');\r\n";
  str += "    ctx2.clearRect(0, 0, c_w, c_h);\r\n";
  str += "    ctx2.beginPath();\r\n";
  str += "    ctx2.lineWidth = line_width;\r\n";
  str += "    ctx2.strokeStyle = '" + frame_col + "';\r\n";
  str += "    ctx2.strokeRect(0,0,c_w,c_h);\r\n";

  str += "    canvas2.addEventListener('touchmove', slider_" + slider_id + ", false);\r\n";

  str += "    function slider_" + slider_id + "(event3) {\r\n";
  str += "      event3.preventDefault();\r\n";
  str += "      event3.stopPropagation();\r\n";
  str += "      var evt555=event3.touches[0];\r\n";
  str += "      var OffSet2 = evt555.target.getBoundingClientRect();\r\n";
  str += "      var ex = evt555.clientX - OffSet2.left;\r\n";
  str += "      if( ex < 0 ){ex = 0;}\r\n";
  str += "      else if(ex>c_w){ex = c_w;}\r\n";
  str += "      var e_cl_X = Math.floor(ex);\r\n";
  str += "      ctx2.clearRect(0, 0, c_w, c_h);\r\n";
  str += "      ctx2.beginPath();\r\n";
  str += "      ctx2.fillStyle = '" + fill_col + "';\r\n";
  str += "      ctx2.rect(0,0, e_cl_X, c_h);\r\n";
  str += "      ctx2.fill();\r\n";
  str += "      ctx2.beginPath();\r\n";
  str += "      ctx2.lineWidth = line_width;\r\n";
  str += "      ctx2.strokeStyle = '" + frame_col + "';\r\n";
  str += "      ctx2.strokeRect(0,0,c_w,c_h);\r\n";
  str += "      doSend(e_cl_X, '" + slider_id + "');\r\n";
  str += "    };\r\n";
  str += "  };\r\n";
  str += "</script>\r\n";
  return str;
}