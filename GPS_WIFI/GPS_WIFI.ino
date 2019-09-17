#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(0, 2); // Wemos D1 Mini D3 (GPIO 0) TX, D4 (GPIO 2) RX

StaticJsonBuffer<300> JSONbuffer;
HTTPClient http;
JsonObject& JSONencoder = JSONbuffer.createObject();
char JSONmessageBuffer[300], charbuf[76], cmd[] = "$GPGGA";
String buf, fail = "fail";
int i;

void changeBaudRate(bool c){
  if(c){
    Serial.end();
    Serial.begin(9600);
    gpsSerial.begin(9600);
  }
  else{
    Serial.end();
    gpsSerial.end();
    Serial.begin(115200);
  }
  delay(1000);
}

void findLocation() {
  changeBaudRate(1);
  JSONencoder["location"] = "fail";
  while (gpsSerial.available()) {
    char val = gpsSerial.read();
    bool isGPGGA = 1;
    if (val == '\n') {
      for (i = 0; i < 6; i++)
        isGPGGA &= (cmd[i] == buf[i]);
      if (isGPGGA)
        JSONencoder["location"] = buf;
      buf = "";
    }
    else
      buf += val;
  }
  changeBaudRate(0);
}

void setup() {
  // Set HTTP and json packet
  JSONencoder["deviceID"] = "AAAAAAAAAAAAAAAA";
  http.begin("http://d-o.o-r.kr");      //Specify request destination
  http.addHeader("Content-Type", "text/plain");  //Specify content-type header
}

void loop() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    findLocation();
    if (fail != JSONencoder["location"]) {
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

      // Post packet and get response
      int httpCode = http.POST(JSONmessageBuffer);   //Send the request
      String payload = http.getString();             //Get the response payload

      Serial.println(JSONmessageBuffer);
      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload);    //Print request response payload
      delay(3000);  //Send a request every 30 seconds
    }
    else {
      Serial.println("GPS fail");
    }
  }
  else {
    Serial.begin(115200); // 시리얼 포트 열고 보드레이트 설정
    WiFi.begin("네??", "hello world");  //와이파이 연결

    while (WiFi.status() != WL_CONNECTED) { // 와이파이 연결까지 대기
      delay(500);
      Serial.println("Waiting for WiFi connection");
    }

    Serial.println("WiFi connected.");
  }
  delay(500);
}
