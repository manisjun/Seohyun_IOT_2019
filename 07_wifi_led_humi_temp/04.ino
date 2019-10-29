#include <OzOLED.h>

#include <SoftwareSerial.h>

#include <DHT.h>
#include <DHT_U.h>

#define DHT_PIN A0  //온습도 측정용
#define DHTTYPE DHT11

#define DEBUG true 
SoftwareSerial esp8266(9,10);

// 함수 millis()
 unsigned long mt1 = 0;
 unsigned long mt2 = 0;

DHT dht(DHT_PIN, DHTTYPE);  //온습도 측정용

// AT커맨드 요청시 응답받는 데이터를 시리얼 모니터에 표시
String sendCommand(String command, const int timeout, boolean debug){
 String response = "";           
 esp8266.print(command); 
 long int time = millis();
 while( (time+timeout) > millis()){
  while(esp8266.available()){        
   char c = esp8266.read(); 
    response+=c;
  }  
 }
 if(debug){
  Serial.print(response);
 }
 return response;
}

void setup(){
 Serial.begin(9600);
 esp8266.begin(9600);

 OzOled.init();
 OzOled.clearDisplay();

 OzOled.printString("BLUE:serverip/a@", 0, 0);
 OzOled.printString("RED:serverip/b@", 0, 1); 
 OzOled.printString("Gre:serverip/c@", 0, 2);
 OzOled.printString("Hu&Te:svrip/t@", 0, 4);

 
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);
 
 digitalWrite(2, LOW);
 digitalWrite(3, LOW);
 digitalWrite(4, LOW);
 
// 공유기와 자동 연결
 sendCommand("AT+RST\r\n",2000,DEBUG); 
 sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); 
 sendCommand("AT+CWJAP=\"yydh269524ap_2\",\"yydh1234\"\r\n",10000,DEBUG); // 공유기(와이파이) ID PW
 sendCommand("AT+CIFSR\r\n",1000,DEBUG); 
 sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG); 
 sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
 Serial.println("Server Ready");
}

void loop(){
// 1시간 마다 esp8266 전원 리셋 및 공유기와 연결 2회 (연결 안정화)
 unsigned long mt2 = millis();
 if(mt2-mt1>3599999){
  sendCommand("AT+RST\r\n",2000,DEBUG); 
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); 
  sendCommand("AT+CWJAP=\"yydh269524ap_2\",\"yydh1234\"\r\n",10000,DEBUG); // 공유기(와이파이) ID PW
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); 
  sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG); 
  sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
  delay(5000);
  sendCommand("AT+RST\r\n",2000,DEBUG); 
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); 
  sendCommand("AT+CWJAP=\"yydh269524ap_2\",\"yydh1234\"\r\n",10000,DEBUG); // 공유기(와이파이) ID PW
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); 
  sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG); 
  sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
  Serial.println("Server Ready");
  mt1=mt2;
 }
 
// Http 통신 기반으로 요청을 받았을 때 (IPD 형식으로!)
 if(esp8266.available() && (esp8266.find("+IPD,"))){
  delay(500);
  
// 요청 메세지에서 원하는 데이터를 String && int 형식으로 변환한다.
  String msg = esp8266.readStringUntil('@');
  Serial.println(msg);
  
  String IDstring = msg.substring(0,2); // String msg 에서 문자 3개를 추출하여, String IDstring 으로 정의할 때, 끝 자리가 \0(널) 로 인하여 없어진다. (결과:두 자리)
  Serial.println(IDstring);
  
// String 형에서 int 형으로 변환
  char IDchar[3]; // 최소 한 개 이상의 배열을 만들어야 정상 작동한다. 숫자3은 그냥 널널하게 정한 것이다. 이전 버전에서 숫자 1을 넣음으로써, 버그를 잡아 주었다. 
  IDstring.toCharArray(IDchar,IDstring.length()); // String IDstring > char IDchar[3] 변환과정에서 \0(널) 로인하여 끝자리 하나가 없어진다. (결과:한 자리)
  Serial.println(IDchar); 
  int ID = atoi(IDchar); // char 형 변수에서 숫자만 추출하여 int 형으로 반환한다. (결과:남은 한 자리 String>int)
  Serial.println(ID); 
  
// 요청 메세지에서 원하는 한글자만 추출한다.
  String msg2 = msg.substring(11,12); // String msg 에서 문자 2개를 추출하여, String msg2 으로 정의할 때, 끝 자리가 \0(널) 로 인하여 없어진다. (결과:한 자리)
  Serial.println(msg2);

  String content;
  
// 요청 메세지에서 원하는 한글자만(문자열이다! 큰 따옴표를 이용하자!) 추출하고 이를 비교하여 원하는 동작을 실행한다.
  if(msg2 == "a"){
   OzOled.clearDisplay();
   OzOled.printString("BLUE:serverip/a@", 0, 0);
   OzOled.printString("BLUE LED HIGH!", 0, 2);
   
   led_write(HIGH, LOW, LOW);

   content = "Blue Led HIGH \n";
   content +="Red Led LOW";
   
  }
  else if(msg2 == "b"){
   OzOled.clearDisplay();
   OzOled.clearDisplay();
   
   OzOled.printString("RED:serverip/b@", 0, 0);
   OzOled.printString("RED LED HIGH!", 0, 2);
   
   led_write(LOW, HIGH, LOW);

   content = "Blue Led LOW \n";
   content +="Red Led HIGH";

  }
  else if(msg2 == "c") {
   OzOled.clearDisplay();

   OzOled.printString("GREEN:serverip/c@", 0, 0);
   OzOled.printString("GREEN LED HIGH!", 0, 2);   

   led_write(LOW, LOW, HIGH);
   
   content = "Green Led HIGH ";
   
   OzOled.printString("GREEN LED LOW! ", 0, 2);   
  }
  
  else if(msg2 == "t") {
   OzOled.clearDisplay();
   OzOled.printString("Hu&Te:serverip/t@", 0, 0);
   
   led_write(LOW, LOW, LOW);  

   /*int A0pinRead = analogRead(A0);
   String A0pinReadString(A0pinRead); //int형을 String형으로 변환
   content = "Cds Read: "; 
   content += A0pinReadString;*/
//온 습도 측정
   int h = dht.readHumidity();
   int t = dht.readTemperature();

//온 습도 LED 표시
   OzOled.printString("Humidity : ", 0, 2);
   OzOled.printNumber((long)h, 11, 2);
   OzOled.printString("Temperature : ", 0, 3);
   OzOled.printNumber((long)t, 13, 3);

//온 습도 표시를 String 으로 형 변환
   String HumidityToString(h);
   String TempToString(t);

   content = "Humidity : ";
   content += HumidityToString;

   content +="\n";
   content += "Temperature : ";
   content += TempToString;
   
  }
 
  sendHTTPResponse(ID,content); // 아래쪽 객체함수를 순서대로 실행한다.
 }
} 

void led_write(int blue, int red, int green){
  digitalWrite(2, blue);
  digitalWrite(3, red);
  digitalWrite(4, green);
}

// http 통신 연결된 ID를 확인하고, Http에 의거 응답코드를 만드는 과정
void sendHTTPResponse(int ID, String content){
 String httpResponse;
 String httpHeader;
 httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n"; 
 httpHeader += "Content-Length: ";
 httpHeader += content.length();
 httpHeader += "\r\n";
 httpHeader +="Connection: close\r\n\r\n";
 
 httpResponse = httpHeader + content + " "; 
 
 sendCIPData(ID,httpResponse);
}

// 만들어놓은 응답코드를 AT커맨더 AT+CIPSEND= 을 통해 발송하기위해 준비한다. 발송할 데이터 길이도 측정한다.
void sendCIPData(int ID, String data){
 String cipSend = "AT+CIPSEND=";
 cipSend += ID;
 cipSend += ",";
 cipSend +=data.length();
 cipSend +="\r\n";
 sendCommand(cipSend,1000,DEBUG);
 
 sendData(data,1000,DEBUG);
}

// 이때까지 만들어놓은 데이터를 발송하고 시리얼 모니터에 발송내용을 표기한 후, 다시 응답받은 데이터를 표시한다.
String sendData(String command, const int timeout, boolean debug){
 String response = "";    
 int dataSize = command.length();
 char data[dataSize];
 command.toCharArray(data,dataSize);          
 
 esp8266.write(data,dataSize); 
 
 if(debug){
  Serial.println("\r\nResponse From Arduino");
  Serial.write(data,dataSize);
 }
 
 long int time = millis();   
 while( (time+timeout) > millis()){
  while(esp8266.available()){
   char c = esp8266.read(); // read the next character.
   response+=c;
  }  
 }
 if(debug){
  Serial.print(response);
 }    
 return response;
}
