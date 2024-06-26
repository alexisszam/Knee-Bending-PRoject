/*
  Author: Ricardo Alexis Zamora Acosta
  Contact Email: zamoraa.alexiss@gmail.com
  Created at: University of Groningen (RUG)
  Date: June 2024
  Description: This code is designed to communicate two Arduinos via Wi-Fi and deploy data in real time.
  Additional Info: Tested with Arduino Nano ESP32, using Arduino IDE version 2.3.2
*/

#include <WiFi.h>

const char *ssid = "ESP32-AP";     // Name of the Wireless Arduino WiFi
const char *password = "password"; // Password for the Wireless Arduino WiFi

WiFiServer server(80);  // Port for the server

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 bps
  
  // Configure the ESP32 as a WiFi access point
  WiFi.softAP(ssid, password);

  // Get the IP address of the WiFi access point
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP adress of the WiFi: ");
  Serial.println(IP);

  server.begin(); // Start the server
}

void loop() {
  WiFiClient client = server.available(); // Check if a client has connected

  if (client) {
    Serial.println("Arduino connected");

    while (client.connected()) {
      if (client.available()) {
        // Read data from the Wireless Arduino
        String data = client.readStringUntil('\n');
        
        // Display the received data in the serial monitor
        Serial.println(data);
      }
    }

    client.stop(); // Stop the client
    Serial.println("Arduino disconnected");
  }
}