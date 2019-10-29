#include <SoftwareSerial.h>       //Including the software serial library
#include <Servo.h>                //including the servo library
SoftwareSerial esp(8, 9);         //RX, TX

#define DEBUG true                //This will display the ESP8266 messages on Serial Monitor
#define servopin 11              //connect servo on pin 8

Servo servo1;                        //variable for servo

int current_position = 180;     
int v = 10;                
int minposition = 0;
int maxposition = 180;
 
//주의 : wifi 접속 AP와 PC의 AP 주소가 같아야 한다.
//1. 주소를 입력하고, Enter키를 친다. 그러면 웹 URL이 변경된다.
//2. 주소를 다시 해당 웹 주소를 선택한 후, servo bar를 조정한다.

void setup()
{

  servo1.attach(servopin);
  servo1.write(maxposition);
  servo1.detach();
  Serial.begin(9600);
  esp.begin(9600);

  sendData("AT+RST\r\n", 2000, DEBUG);                     
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);                  
  sendData("AT+CWJAP=\"yydh269524ap_2\",\"yydh1234\"\r\n", 2000, DEBUG);   //Enter WiFi name and password
  while(!esp.find("OK")) {                                  //this will check for connection
  }

  sendData("AT+CIFSR\r\n", 1000, DEBUG);          //This will print IP address on the serial monitor
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);      
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);   //This will start the web server on port 80
}

 

void loop()

{

  if (esp.available())     // This will check if input is available on ESP8266
  {
    if (esp.find("+IPD,"))     {
      String msg;
      esp.find("?");                           //run cursor until command is found
      msg = esp.readStringUntil(' ');         
      String command = msg.substring(0, 3);   
      String valueStr = msg.substring(4);     
      int value = valueStr.toInt();            //It will convert Value to integer
      if (DEBUG) {
        Serial.println(command);
        Serial.println(value);
      }
      delay(100);

      if(command == "sr1") {
         if (value >= maxposition) {
           value = maxposition;
         }

         if (value <= minposition) {
           value = minposition;
         }

         servo1.attach(servopin);
         while(current_position != value) {
           if (current_position > value) {
             current_position -= 1;
             servo1.write(current_position);
             delay(100/v);
           }

           if (current_position < value) {
             current_position += 1;
             servo1.write(current_position);
             delay(100/v);
           }
         }

         servo1.detach();
      }
    }
  }
}

String sendData(String command, const int timeout, boolean debug)
{

  String response = "";
  esp.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp.available())
    {
      char c = esp.read();
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
