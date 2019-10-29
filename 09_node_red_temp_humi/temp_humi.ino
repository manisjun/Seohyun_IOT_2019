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
  OzOled.printString(" Humi&Temp ck", 0, 0);  

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
void loop() {

  node_red();
  OzO_led();
  delay(500);
    
}
