#include <OzOLED.h>

#include <Ethernet.h>
#include <TextFinder.h>
//https://playground.arduino.cc/Code/TextFinder/
//https://bildr.org/2011/06/arduino-ethernet-pin-control/
#include <SPI.h>

byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(192,168,0,13);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192, 168, 0, 1);
IPAddress dns_local(168,126,63,1);

char buffer[8];

EthernetServer server(80);

void setup() {
  OzOled.init();
  OzOled.clearDisplay();
  
  Serial.begin(9600);
  Ethernet.begin(mac, ip, dns_local, gateway, subnet);
  Serial.print("IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(ip[thisByte], DEC);
    Serial.print("."); 
  }

  ip_display(ip);
  Serial.println();
  server.begin();
}
void loop() {
  EthernetClient client = server.available();
  if (client) {
    TextFinder finder(client);
    int type = 0;
    while (client.connected()) {
      if (client.available()) {
        //http request start with "GET / HTTP/1.1"
        if (finder.getString("","/", buffer, sizeof(buffer))) {
          if(strcmp(buffer, "POST ") == 0) {
            finder.find("\n\r");
            while (finder.findUntil("pinD", "\n\r")) {
              int pin = finder.getValue();
              int val = finder.getValue();
              pinMode(pin, OUTPUT);
              digitalWrite(pin, val);
              
              status_display(pin, val);
              
              Serial.print("pin: ");
              Serial.print(pin);
              Serial.print(" Val: ");
              Serial.println(val);
            }
          }
          
          sendHeader(client,"Post example");
          //create HTML button to control pin 2
          client.println("<h2>Click buttons to turn pin 2 on or off</h2>");
          client.print("<form action='/' method='POST'><p><button type='submit' name='pinD2' value='1'>on</button><p>");
          client.println("<button type='submit' name='pinD2' value='0'>off</button> </form>");
          client.println("</body></html>");
          client.stop();
          break;
          //설정한 핀을 켜고 끄는데 생각보다는 시간이 오래 걸린다. 약 10초 정도
        }//if finder
      }//if client available
     
    }//while connected
   delay(1);
    client.stop();
  }//if client
 
}

void sendHeader(EthernetClient client, char *title) {
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.print("<html><head><title>");
  client.print(title);
  client.println("</title><body>");
}

void ip_display(IPAddress myIP_1){
  OzOled.printString("My IP address:", 0, 0);
  OzOled.printNumber((long)myIP_1[0], 1, 2);
  OzOled.printString(".", 4, 2);
  OzOled.printNumber((long)myIP_1[1], 5, 2);
  OzOled.printString(".", 8, 2);
  OzOled.printNumber((long)myIP_1[2], 9, 2);
  OzOled.printString(".", 12, 2);
  OzOled.printNumber((long)myIP_1[3], 13, 2);
}

void status_display(int pin, int val) {
  OzOled.printString("pin: ", 0, 4);
  OzOled.printNumber((long)pin, 6, 4);
  OzOled.printString("val : ", 8, 5);
  OzOled.printNumber((long)val, 14, 5);
}
