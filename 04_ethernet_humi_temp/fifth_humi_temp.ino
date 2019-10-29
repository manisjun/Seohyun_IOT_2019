#include <OzOLED.h>

#include <SPI.h>
#include <Ethernet.h>

#include <DHT.h>
#include <DHT_U.h>

#define DHT_PIN A0
#define DHTTYPE DHT11

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

DHT dht(DHT_PIN, DHTTYPE);

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

  int h = dht.readHumidity();
  int t = dht.readTemperature();
  
  OzOled.printString("humidity : ", 0, 5);
  OzOled.printNumber((long)h, 11, 5);
  OzOled.printString("temperature : ", 0, 4);
  OzOled.printNumber((long)t, 13, 4);
    
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
          
          //web_display(client, h, t);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<meta http-equiv=\"refresh\" content=\"2\">");

          client.println("<title>");
          client.print("Welcome EDUINO");
          client.println("</title>");

          client.println("<style>");

          client.println(".container {");
          client.println("  width : 600px;");
          client.println("  min-height : 200px;");
          client.println("  margin : 0 auto;");
          client.println("  background-color : rgba(230,234,155,0.3);");
          client.println("  border : 3px solid #545104;");
          client.println("  border-radius : 10px;");
          client.println("}");
          client.println("h1 {");
          client.println("  color : tomato;");
          client.println("}");

          

          client.println("</style>");
          client.println("<body>");
          
          client.println("<div class = \"container\">");
          client.println("<center>");
          
          client.println("<h1>");
          client.print("SeoHyun House");
          client.println("</h1>");

          client.println("<h2>");
          client.print("Office Temperature and Humidity");
          client.println("</h2>");

          client.println("<h4>");
          client.print("Temperature : ");
          client.print(t);
          client.print("<sup>o</sup>");
          client.print("C");
          client.println("<br>");
          client.print("Humidity : ");
          client.print(h);
          client.print("%");
          client.println("</h4>");
          
          client.println("</center>");
          client.println("</div>");

          client.println("</body>");
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

/*void web_display(EthernetClient client, int h, int t) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.println("<title>");
          client.print("Welcome EDUINO");
          client.println("</title>");

          client.println("<center>");
          
          client.println("<h1>");
          client.print("EDUINO Office");
          client.println("</h1>");

          client.println("<h2>");
          client.print("Office Temperature and Humidity");
          client.println("</h2>");

          client.println("<h4>");
          client.print("Temperature : ");
          client.print(t);
          client.print("<sup>o</sup>");
          client.print("C");
          client.println("<br>");
          client.print("Humidity : ");
          client.print(h);
          client.print("%");
          client.println("</h4>");
          
          client.println("</center>");

          client.println("</html>");
}*/
