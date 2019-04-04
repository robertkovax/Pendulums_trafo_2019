#include <Arduino.h>
#include <WiFi.h>
#include "pendulum.hpp"

// Replace with your network credentials
const char* ssid     = "pendulum";
const char* password = "123456789";

WiFiServer server(80);

String header;

String DirState = "off";
String PWMState = "off";

const int Dir = 22;
const int PWM = 21;


IPAddress local_IP(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

int value = 255;
String strValue = "";

void setup() {
  Serial.begin(9600);
  
  // led pwm setup
  ledcSetup(ledchannel14, freq, resolution);
  ledcSetup(ledchannel15, freq, resolution);

  ledcAttachPin(Dir, ledchannel14);
  ledcAttachPin(PWM, ledchannel15);

  // network config
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
    // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){





  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /14/on") >= 0) {
              Serial.println("GPIO 14 on");
              DirState = "on";
              ledcWrite(ledchannel14, value);
            } else if (header.indexOf("GET /14/off") >= 0) {
              Serial.println("GPIO 14 off");
              DirState = "off";
              ledcWrite(ledchannel14, 0);
            } else if (header.indexOf("GET /15/on") >= 0) {
              Serial.println("GPIO 15 on");
              PWMState = "on";
              ledcWrite(ledchannel15, value);
            } else if (header.indexOf("GET /15/off") >= 0) {
              Serial.println("GPIO 15 off");
              PWMState = "off";
              ledcWrite(ledchannel15, 0);
            } else if (header.indexOf("GET /setValue/") >= 0) {
              int getIndex = header.indexOf("GET /setValue/");
              int getEndIndex = header.indexOf(" HTTP", getIndex);
              strValue = header.substring(getIndex + 14, getEndIndex);
              value = strValue.toInt();
              if (value > 255) value = 255;
              if (value < 0) value = 0;
              if (DirState == "on") ledcWrite(ledchannel14, 255);
              if (PWMState == "on") ledcWrite(ledchannel15, value);
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            client.println("<body><h1>ESP32 Web Server</h1>");

            client.print("<p>Value: ");
            client.print(value);
            client.println("</p>");
            
            client.println("<p>Dir - State " + DirState + "</p>");
            if (DirState=="off") {
              client.println("<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            client.println("<p>PWM - State " + PWMState + "</p>");
            if (PWMState=="off") {
              client.println("<p><a href=\"/15/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/15/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
