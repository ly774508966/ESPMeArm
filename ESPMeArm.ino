#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <WiFiUdp.h>

#define DEBUG 0
#define MAX_BUFFER_SIZE 40

const char* wifi_ssid = "SkyNet";
const char* wifi_password = "alphabetasoup";
const IPAddress wifi_remoteAddress(192, 168, 5, 124);

WiFiUDP AT;
const int udp_port = 5556;

char inBuffer[MAX_BUFFER_SIZE] = {0};

void connectToWifi();

void setup() {
  Serial.begin(115200);
  connectToWifi();
}

void loop() {
  while (WiFi.status() == WL_CONNECTED) {
    if (Serial.available()) {
      char inChar = Serial.read();
      if (inChar != '\n') {
        strncat(inBuffer, &inChar, 1);
        Serial.print(inChar);
      } else {
        Serial.println();
        // Full string received, transmit through wifi        
        AT.beginPacket(wifi_remoteAddress, udp_port);
        AT.write(inBuffer);
        AT.endPacket();

        memset(inBuffer, 0, MAX_BUFFER_SIZE);
      }
    }
  }
  Serial.println("disconnected from WiFi, attempting to reconnect");
  connectToWifi();
}

void connectToWifi() {
  // Connect to WiFi network
  Serial.println("connecting to WiFi");
  WiFi.mode(WIFI_STA);

  WiFi.disconnect();
  while (WiFi.status() != WL_IDLE_STATUS) {
    // delay while/if connected to network
    delay(1000);
    Serial.println(WiFi.status());
  }

  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  Serial.print("Connected, address=");
  Serial.println(WiFi.localIP()); 
  
  // Prep UDP
  Serial.println("Starting UDP...");
  AT.begin(udp_port);
  AT.flush();
}

void setBit(unsigned int& status_var, unsigned int mask) {
  status_var |= mask;
}

void clearBit(unsigned int& status_var, unsigned int mask) {
  status_var &= ~mask;
}

void toggleBit(unsigned int& status_var, unsigned int mask) {
  status_var ^= mask;
}

