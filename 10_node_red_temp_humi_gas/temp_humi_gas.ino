#include <DHT.h>
#include <DHT_U.h>
#include <OzOLED.h>

#define DHT_PIN A0
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);

void setup() {

  Serial.begin(9600);
  OzOled.init();
  OzOled.clearDisplay();
  OzOled.printString("Humi&Temp ck", 0, 0); 
  OzOled.printString("Gas : ", 0, 5);  //열, 행 

  pinMode(2, OUTPUT);

}
void node_red(){
  float hum = dht.readHumidity(); //float 형으로 선언하면 OzOled 화면에 깨져서 나옴.
  float temp = dht.readTemperature();
  
  String hum1 = (String)hum;
  String temp1 = (String)temp; 
  
  Serial.print(hum1);
  Serial.print(",");
  Serial.print(temp1);
}

void OzO_led(){
  int hum = dht.readHumidity();
  int temp = dht.readTemperature();

  OzOled.printString("Humidity : ", 0, 2);
  OzOled.printNumber((long)hum, 11, 2);
  OzOled.printString("Temperature : ", 0, 3);
  OzOled.printNumber((long)temp, 13, 3);
  
}

void gas() {
  int value= analogRead(A1);//reads the analaog value from the CO sensor's AOUT pin
  Serial.print(",");
  Serial.print(value);//prints the CO value
  //이로인해 하나의 문자열 형태로 습도와 온도, 가스센서 값이 서로 콤마로 연결되어 표시됨.
  //이를 Node-Red에서는 배열로 받아들여, 각 배열인자마다 -48을 하여 다시 숫자로 변환함.
  Serial.print(",");

  if(value < 100) {
    OzOled.printString("    ", 7, 5);
    OzOled.printNumber((long)value, 7, 5);
  }
  else
    OzOled.printNumber((long)value, 7, 5);
    
  if(value >= 300) {
    digitalWrite(2, HIGH);
    OzOled.printString("Warning!!", 0, 7);
  }
  else {
    digitalWrite(2, LOW);
    OzOled.printString("Safed  !!", 0, 7);
  }
}

void loop() {

  node_red();
  OzO_led();
  gas();
  delay(500);
    
}
