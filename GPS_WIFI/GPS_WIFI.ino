#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(0, 2); // Wemos D1 Mini D3 (GPIO 0) TX, D4 (GPIO 2) RX

char JSONmessageBuffer[300], charbuf[76], cmd[] = "$GPGGA";
String buf, tmp, fail = "fail";
int i;

void findLocation() {
  tmp = "fail";
  while (gpsSerial.available()){
    char val = gpsSerial.read() ;
    bool isGPGGA = 1;
    if (val == '\n'){
      for(i=0; i<6; i++)
        isGPGGA &= (cmd[i]==buf[i]);
      if(isGPGGA)
        tmp = buf;
      buf="";
    }
    else
      buf += val;
  }
}

void setup() {
  // Set HTTP and json packet
  gpsSerial.begin(9600);
  Serial.begin(115200);
}

void loop() {
  //Check WiFi connection status
  findLocation();
  if (WiFi.status() == WL_CONNECTED) {
    if (fail != tmp) {
      StaticJsonBuffer<300> JSONbuffer;
      JsonObject& JSONencoder = JSONbuffer.createObject();
      JSONencoder["location"] = tmp;
      JSONencoder["deviceID"] = "AAAAAAAAAAAAAAAA";
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      
      Serial.println(tmp);
      Serial.println(JSONmessageBuffer);
      
      HTTPClient http;
      http.begin("http://d-o.o-r.kr/insert");      //Specify request destination
      http.addHeader("Content-Type", "application/json");  //Specify content-type header
      
      // Post packet and get response
      int httpCode = http.POST(JSONmessageBuffer);   //Send the request
      String payload = http.getString();             //Get the response payload

      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload);    //Print request response payload
      
      http.end();
    }
  }
  else {
    WiFi.begin("A", "aspirinsmj");  //와이파이 연결
    while (WiFi.status() != WL_CONNECTED) { // 와이파이 연결까지 대기
      delay(500);
      Serial.println("Waiting for WiFi connection");
      continue;
    }
    Serial.println("WiFi connected.");
  }
}
