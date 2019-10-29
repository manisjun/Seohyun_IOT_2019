#include <OzOLED.h>

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 //mac 주소는 학생들마다 서로 다르게 지정한다. 실습용 보드는 mac 주소가 기재되어 있지 않다.
};

EthernetServer server(80);

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
  IPAddress myIP = Ethernet.localIP();
  // print your local IP address:
  Ethernet.begin(mac, myIP);

  server.begin();
  Serial.print("server is at ");
  Serial.println(myIP);
  ip_display(myIP);
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<br>");          
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);           
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br>");
          }
          for (int digitalChannel = 0; digitalChannel < 14; digitalChannel++) {
            int sensorReading = digitalRead(digitalChannel);
            client.print("Digital output ");
            client.print(digitalChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br>");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void ip_display(IPAddress myIP_1){
  OzOled.printString("Server IP addr :", 0, 0);
  OzOled.printNumber((long)myIP_1[0], 1, 2);
  OzOled.printString(".", 4, 2);
  OzOled.printNumber((long)myIP_1[1], 5, 2);
  OzOled.printString(".", 8, 2);
  OzOled.printNumber((long)myIP_1[2], 9, 2);
  OzOled.printString(".", 12, 2);
  OzOled.printNumber((long)myIP_1[3], 13, 2);
}
