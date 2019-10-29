#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10); //RX, TX - 이는 보드 연결과 반대로 기재한다.
//와이파이 모듈은 5v로 연결하여야 하며, 이는 아두이노보드 자체에 연결해야 한다.

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

}

void loop() {
  if(mySerial.available()) {
    Serial.write(mySerial.read());
   // Serial.println("ok");
  }
  if(Serial.available()) {
    mySerial.write(Serial.read());
  }

}
