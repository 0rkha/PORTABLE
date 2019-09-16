#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(0,2); // Wemos D1 Mini D3 (GPIO 0) RX, D4 (GPIO 2) TX
String buf;
char charbuf[76];
int i;
int val;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  delay(100);
  if (gpsSerial.available()){
    char val2 = gpsSerial.read() ;
    buf += val2;
    if (val2 == '\n'){
      Serial.println(buf);
      buf="";
    }
  }if (gpsSerial.available()){
    char val2 = gpsSerial.read() ;
    buf += val2;
    if (val2 == '\n'){
      Serial.println(buf);
      buf="";
    }
  }
}

void loop() {
  
}
