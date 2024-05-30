#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BluetoothSerial SerialBT;

String teamId = "A60";
const char* btName = "CustomESP32Name";

void setup() {
  Serial.begin(115200);

  // Set up Bluetooth with custom name
  SerialBT.begin(btName); // Bluetooth device name
  Serial.println("The device started, now you can pair it with Bluetooth!");

  if(!display.begin(SSD1306_PAGEADDR, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  if (SerialBT.available()) {
    String incomingMessage = SerialBT.readString();
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, incomingMessage);

    if (error) {
      Serial.println("Failed to parse incoming message");
      return;
    }

    String action = doc["action"];
    if (action == "getLCDs") {
      String incomingTeamId = doc["teamId"];
      handleGetLCDs(incomingTeamId);
    } else if (action == "setText") {
      int id = doc["id"];
      JsonArray textArray = doc["text"];
      handleSetText(id, textArray);
    } else if (action == "setImage") {
      int id = doc["id"];
      JsonArray data = doc["data"];
      handleSetImage(id, data);
    } else if (action == "scroll") {
      int id = doc["id"];
      String direction = doc["direction"];
      handleScroll(id, direction);
    } else if (action == "clearDisplay") {
      int id = doc["id"];
      handleClearDisplay(id);
    }
  }
}

void handleGetLCDs(String incomingTeamId) {
  teamId = incomingTeamId;
  StaticJsonDocument<256> doc;
  JsonObject lcd = doc.createNestedObject();
  lcd["type"] = "5110";
  lcd["interface"] = "SPI";
  lcd["resolution"] = "84x48";
  lcd["id"] = 2;
  lcd["teamId"] = teamId;
  
  String response;
  serializeJson(doc, response);
  SerialBT.println(response);
}

void handleSetText(int id, JsonArray textArray) {
  if (id == 1) {
    display.clearDisplay();
    for (int i = 0; i < textArray.size(); i++) {
      display.setCursor(0, i * 8);
      display.print(textArray[i].as<String>());
    }
    display.display();
    
    StaticJsonDocument<256> doc;
    doc["id"] = id;
    doc["text"] = textArray;
    doc["teamId"] = teamId;
    
    String response;
    serializeJson(doc, response);
    SerialBT.println(response);
  }
}

void handleSetImage(int id, JsonArray data) {
  if (id == 1) {
    display.clearDisplay();
    for (JsonObject pixel : data) {
      int x = pixel["x"];
      int y = pixel["y"];
      display.drawPixel(x, y, SSD1306_WHITE);
    }
    display.display();
    
    StaticJsonDocument<256> responseDoc;
    responseDoc["id"] = id;
    responseDoc["number_pixels"] = data.size();
    responseDoc["teamId"] = teamId;
    
    String response;
    serializeJson(responseDoc, response);
    SerialBT.println(response);
  }
}

void handleScroll(int id, String direction) {
  if (id == 1) {
    if (direction == "Left") {
      display.startscrollleft(0x00, 0x0F);
    } else if (direction == "Right") {
      display.startscrollright(0x00, 0x0F);
    } else {
      display.stopscroll();
    }
    
    StaticJsonDocument<256> doc;
    doc["id"] = id;
    doc["scrolling"] = direction;
    doc["teamId"] = teamId;
    
    String response;
    serializeJson(doc, response);
    SerialBT.println(response);
  }
}

void handleClearDisplay(int id) {
  if (id == 1) {
    display.clearDisplay();
    display.display();
    
    StaticJsonDocument<256> doc;
    doc["id"] = id;
    doc["cleared"] = true;
    doc["teamId"] = teamId;
    
    String response;
    serializeJson(doc, response);
    SerialBT.println(response);
  }
}
