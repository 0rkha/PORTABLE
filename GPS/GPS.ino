#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(0,2); // Wemos D1 Mini D3 (GPIO 0) RX, D4 (GPIO 2) TX
String buf;
char charbuf[76];
char cmd[] = "$GPGGA";
int i;
int val;

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  delay(100);
}

void loop() {
  while (gpsSerial.available()){
    char val2 = gpsSerial.read() ;
    bool isGPGGA = 1;
    if (val2 == '\n'){
      for(i=0; i<6; i++)
        isGPGGA &= (cmd[i]==buf[i]);
      if(isGPGGA)
        Serial.println(buf);
      buf="";
    }
    else
      buf += val2;
  }
}
