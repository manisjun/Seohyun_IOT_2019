#include <boarddefs.h>
#include <IRremote.h>

#include <ir_Lego_PF_BitStreamEncoder.h>

#include <SimpleTimer.h>


#include <OzOLED.h>

#include <DHT.h>
#include <DHT_U.h>
//실행 시 라이브러리 설치가 안되어서 실행이 안되면, 파일-환경설정-스케치북 위치의 libraries 폴더에 붙여넣는다.
#define DHT_PIN A0
#define DHTTYPE DHT11

SimpleTimer timer;

int red_pin = 6;
int green_pin = 7;
int blue_pin = 8;
int Recv_pin = A2;
int Pir_pin = 5;
int gas_blue_pin = 2;
int gas_red_pin = 3;
int Relay_pin = 4;

int Humi_timer;
int Motion_timer;
int Gas_timer;
int Led_timer;

bool sw_temp = true;
bool sw_motion = true;
bool sw_gas = true;
bool sw_gas_led = true;

DHT dht(DHT_PIN, DHTTYPE);
IRrecv irrecv(Recv_pin);
decode_results results;


void setup() {
  OzOled.init();
  OzOled.clearDisplay();
  irrecv.enableIRIn();

  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);                    
  pinMode(blue_pin, OUTPUT);
  pinMode(gas_blue_pin, OUTPUT);
  pinMode(gas_red_pin, OUTPUT);
  pinMode(Relay_pin, OUTPUT);
  pinMode(Pir_pin, INPUT);
  

  OzOled.printString(" 0:Humi&Temp ck", 0, 0);
  OzOled.printString(" 1:Motion check", 0, 1);
  OzOled.printString(" 4:Gas check", 0, 2);
  OzOled.printString(" 2:Relay_Low", 0, 3);
  OzOled.printString(" 5:Relay_High", 0, 4);
  
  timer_setup();
}

void loop() {
  if (irrecv.decode(&results)) {
    input_check();
    irrecv.resume(); // 다음 값을 받는다.
  }
  
    timer.run();
}

void timer_setup(){
  
  Humi_timer = timer.setInterval(100, readHumi_Temp);
  Motion_timer = timer.setInterval(50, readMotion);
  Gas_timer = timer.setInterval(100, readGas);
  Led_timer = timer.setInterval(500, emergency);
 
  timer.disable(Humi_timer);
  timer.disable(Motion_timer);
  timer.disable(Gas_timer);
  timer.disable(Led_timer);  
}

void input_check(){
     switch (results.value) { // 리모콘 버튼의 고유값에 따라
      case 0xFF6897 :  // 리모콘의 0버튼이 눌리면, LED_RED ON
        OzOled.clearDisplay();
        LED_OLED(1, 0, 0, 0, 0xFF6897);
        OzOled.printString("Humidity and", 0, 0);
        OzOled.printString("Temp check", 0, 1);
        
        timer.enable(Humi_timer);
        timer.disable(Motion_timer);
        timer.disable(Gas_timer);
        timer.disable(Led_timer);
        
        break;
      case 0xFF30CF :  // 리모콘의 1버튼이 눌리면, LED_RED ON
        OzOled.clearDisplay();
        LED_OLED(0, 1, 0, 1, 0xFF30CF);
        OzOled.printString("Motion Check", 0, 0);

        timer.enable(Motion_timer);
        timer.disable(Humi_timer);
        timer.disable(Gas_timer);
        timer.disable(Led_timer);
        
        
        break;
      case 0xFF10EF : // 리모콘의 4버튼이 눌리면, LED OFF
        OzOled.clearDisplay();
        LED_OLED(0, 0, 1, 4, 0xFF10EF);
        OzOled.printString("Gas Check", 0, 0);

        timer.enable(Gas_timer);
        timer.disable(Humi_timer);
        timer.disable(Motion_timer);
        timer.disable(Led_timer);
        break;
      case 0xFF42BD : // 리모콘의 7버튼이 눌리면, LED OFF
        LED_OLED(0, 0, 1, 7, 0xFF42BD);
        break;
      case 0xFF18E7: // 리모콘의 2버튼이 눌리면, LED_YELLOW ON
        timer.disable(Humi_timer);
        timer.disable(Motion_timer);
        timer.disable(Gas_timer);
        timer.disable(Led_timer); 
        
        OzOled.clearDisplay();
        OzOled.printString("Relay : LOW  ", 0, 1);      
        LED_OLED(1, 1, 0, 2, 0xFF18E7);
        digitalWrite(Relay_pin, LOW);
        break;
      case 0xFF38C7: // 리모콘의 5버튼이 눌리면, LED OFF
        timer.disable(Humi_timer);
        timer.disable(Motion_timer);
        timer.disable(Gas_timer);
        timer.disable(Led_timer); 
              
        OzOled.clearDisplay();
        OzOled.printString("Relay : HIGH", 0, 1);              
        LED_OLED(1, 0, 1, 5, 0xFF38C7);
        digitalWrite(Relay_pin, HIGH);
        break;
      case 0xFF4AB5: // 리모콘의 8버튼이 눌리면, LED OFF
        LED_OLED(0, 1, 1, 8, 0xFF4AB5);
        break;
      case 0xFF7A85: // 리모콘의 3버튼이 눌리면, LED_BLUE ON
        LED_OLED(1, 1, 1, 3, 0xFF7A85);
        break;
      case 0xFF5AA5: // 리모콘의 6버튼이 눌리면, LED_BLUE ON
        LED_OLED(1, 0, 1, 6, 0xFF5AA5);
        break;
      case 0xFF52AD: // 리모콘의 9버튼이 눌리면, LED_BLUE ON
        timer.disable(Humi_timer);
        timer.disable(Motion_timer);
        timer.disable(Gas_timer);
        timer.disable(Led_timer);
        OzOled.clearDisplay();
        LED_OLED(0, 0, 1, 9, 0xFF52AD);
        OzOled.printString(" 0:Humi&Temp ck", 0, 0);
        OzOled.printString(" 1:Motion check", 0, 1);
        OzOled.printString(" 4:Gas check", 0, 2);
        OzOled.printString(" 2:Relay_Low", 0, 3);
        OzOled.printString(" 5:Relay_High", 0, 4);
        break;
    } 
}


void readHumi_Temp(){

  if(sw_temp == true){
    readHumi_Temp_check();
  }
  else{
    readHumi_Temp_check();
  }
  sw_temp = !sw_temp;
}

void readHumi_Temp_check(){
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  
  OzOled.printString("Humidity : ", 0, 2);
  OzOled.printNumber((long)h, 11, 2);
  OzOled.printString("Temperature : ", 0, 3);
  OzOled.printNumber((long)t, 13, 3);
}

void readMotion(){
  if(sw_motion == true){
    readMotion_check();
  }
  else{
    readMotion_check();
  }
  sw_motion = !sw_motion;  
}

void readMotion_check(){
  int motion_detect = digitalRead(Pir_pin);
  OzOled.printString("Motion Value", 0, 2);
  OzOled.printNumber((long)motion_detect, 13, 2);
  
  if(motion_detect == HIGH){
    OzOled.printString("Motion Detected", 0, 3);
    OzOled.printString("         ", 6, 4);   
    digitalWrite(red_pin, HIGH);
    digitalWrite(green_pin, LOW);
    digitalWrite(blue_pin, LOW);
  }
  else {
    OzOled.printString("Motion Not     ", 0, 3);
    OzOled.printString("Detected", 6, 4);    
    digitalWrite(red_pin, LOW);
    digitalWrite(green_pin, HIGH);
    digitalWrite(blue_pin, LOW);
 }
}


void readGas(){
  if(sw_gas == true){
    readGas_check();
  }
  else{
    readGas_check();
  }
  sw_gas = !sw_gas; 
}

void readGas_check(){
  int gas_detect = analogRead(A1);
  OzOled.printString("Gas : ", 0, 2);
  OzOled.printNumber(long(gas_detect), 7, 2);
  if(gas_detect >= 200) {
    OzOled.printString("Gas detected!!!", 0, 3);   
    for(int i = 0; i <=5; i++){
      timer.enable(Led_timer);
    }
  }
  else{
    digitalWrite(gas_red_pin, LOW);
    digitalWrite(gas_blue_pin, HIGH);
    OzOled.printString("                ", 0, 3);
    timer.disable(Led_timer);  
 }

 if(gas_detect < 100) {
    OzOled.printString(" ", 9, 2);
 }
}

void LED_OLED(int red, int green, int blue, long num, long button){
  digitalWrite(red_pin, red);
  digitalWrite(green_pin, green);
  digitalWrite(blue_pin, blue);
  OzOled.printNumber(num, 0, 6);
  OzOled.printString(" bun : ", 1, 6);
  OzOled.printNumber(button, 7, 6);
  OzOled.printString("     9 : Restart", 0, 7);  
}

void emergency(){
  if(sw_gas_led == true){
    digitalWrite(gas_red_pin, LOW);
    digitalWrite(gas_blue_pin, HIGH);
  }
  else{
    digitalWrite(gas_red_pin, HIGH);
    digitalWrite(gas_blue_pin, LOW);
  }
  sw_gas_led = !sw_gas_led;
}
      
