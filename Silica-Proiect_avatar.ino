#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "WiFi.h"
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  String raspuns = "<!DOCTYPE html><html><head><title>Proiect PIA</title><style>";
  raspuns += "body { background-color: coral;}";
  raspuns += "</style></head><body><h1>Rick and Morty</h1>";

  // Begin HTTP request to get the list of characters
  HTTPClient http;
  http.begin("http://proiectia.bogdanflorea.ro/api/rick-and-morty/characters");

  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      // Read and parse the JSON response
      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, http.getString());
      if (!error) {
        // Iterate over each character in the JSON
        JsonArray data = doc.as<JsonArray>();
        for (JsonObject character : data) {
          // Retrieve id as long
          long id = character["id"];
          const char *name = character["name"];
          const char *url = character["img_url"];

          
          raspuns += "<h2>id: ";
          raspuns += String(id);
          raspuns += "</h2>";
          
          raspuns += "<h3>Name: ";
          raspuns += name;
          raspuns += " ";
          raspuns += "</h3>";

          raspuns += "<img src='";
          raspuns += url;
          raspuns += "' style='max-width: 500px;'>";

        }
      } else {
        raspuns += "Error parsing JSON: ";
        raspuns += error.c_str();
      }
    } else {
      raspuns += "HTTP error: ";
      raspuns += httpCode;
    }
  } else {
    raspuns += "Error connecting to server.";
  }
  http.end();
  raspuns += "</body></html>";
  server.send(200, "text/html", raspuns);
}

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
  
  Serial.println("Scan start");
 
    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");

        Serial.println("Nr | SSID                             | RSSI | CH | Metoda criptare");
        for (int i = 0; i < n; ++i) {

            // Print SSID and RSSI for each network found

            Serial.printf("%2d",i + 1);
            Serial.print(" | ");

            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");

            Serial.printf("%4d", WiFi.RSSI(i));
            Serial.print(" | ");

            Serial.printf("%2d", WiFi.channel(i));
            Serial.print(" | ");

            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");
 
    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

  // Connect to WiFi network
  Serial.println("Connecting to WiFi...");

  WiFi.begin("Telefon_Alex", "alexalex");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");

  Serial.print("IP Address: ");

  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
}

void loop() {
  server.handleClient();
}
