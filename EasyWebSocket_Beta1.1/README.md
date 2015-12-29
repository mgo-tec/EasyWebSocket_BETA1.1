# EasyWebSocket Beta version1.1
Simple library for WebSocket communication with ESP-WROOM-02(ESP8266) and smartphone browser.

This requires the Arduino core for ESP8266 WiFi chip library.
https://github.com/esp8266/arduino

This library is a simple one that specializes in ON-OFF button and the slider(range) and Text display of the browser.

Using the latest smart phone, please use as much as possible high-speed CPU, large memory, high-speed network. Otherwise, there are times when traffic occurs to communicate cut off overflow.

It may interrupted communication in Safari on iOS. Cause I do not know yet.
You will be able to communicate Google Chrome if probably comfortable with the latest high-spec Android smartphone.

You must write a JavaScript header in SPIFFS FileSystem to ESP8266 flash before compilation.
The header can be found in the following location.

EasyWebSocket_Beta1.1\examples\EasyWebSocket_testB1.1\data\spiffs_01.txt

Please refer to here upload method.
https://github.com/esp8266/arduino-esp8266fs-plugin

# Change log:
1.Add a graphical slider using the Canvas element.
2.Add thinning transmission time selection button.
3.Change multiple function specification.

# Credits and license
*Licensed under MIT 
*Copyright (c) 2015 mgo-tec.com 
Other usage is Japanese, please visit my blog.

My Blog: 

https://www.mgo-tec.com