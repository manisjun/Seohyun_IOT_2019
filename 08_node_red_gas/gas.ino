#include <OzOLED.h>

const int AOUTpin= 1;//the AOUT pin of the CO sensor goes into analog pin A0 of the arduino
int value;
//Node-Red 활용 시 SerialPort를 지정해 놓으면 충돌이 발생하여 업로드 되지 않는다. 따라서, Node-Red의 포트 번호를 다른 번호로 변경 후, 업로드 필요
void setup() 
{
  OzOled.init();
  OzOled.clearDisplay();
  OzOled.printString("Gas : ", 1, 2);  //열, 행
  Serial.begin(9600);//sets the baud rate
  pinMode(AOUTpin, INPUT);//sets the pin as an input to the arduino
  pinMode(2, OUTPUT);
}

void loop()
{
  value= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
  //Serial.print("CO value: ");
  Serial.print(value);//prints the CO value, 문자열 형태로 표시하여 Node-red에서 활용
  //배열로 받을 수 있으므로, 숫자에 해당하는 문자부분만 숫자로 다시 변환(-48)하여 활용할 수 있음.

  if(value < 100) {
    OzOled.printString("    ", 7, 2);
    OzOled.printNumber((long)value, 7, 2);
  }
  else
    OzOled.printNumber((long)value, 7, 2);
    
  if(value >= 300) {
    digitalWrite(2, HIGH);
    OzOled.printString("Warning!!", 1, 4);
  }
  else {
    digitalWrite(2, LOW);
    OzOled.printString("Safed  !!", 1, 4);
  }
  
  delay(600);
}
