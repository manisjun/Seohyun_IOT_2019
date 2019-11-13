#include <OzOLED.h>

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 //mac 주소는 임의로 주는 것이므로, 맨 끝의 0x02는 임의수를 배정하여 학생들마다 서로 다르게 지정한다.
};

void setup() {
  OzOled.init();
  OzOled.clearDisplay();
  
  Serial.begin(9600);
  
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true) {
      delay(1);
    }
  }
  
  // print your local IP address:
  IPAddress myIP = Ethernet.localIP();
  Serial.print("My IP address: ");
  Serial.println(myIP);
  ip_display(myIP);
  
}

void loop() {
 
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
