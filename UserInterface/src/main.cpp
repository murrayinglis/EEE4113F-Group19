 /*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-plot-readings-charts-multiple/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <SD.h> // Include the SD library for SD card functionality
#include <Arduino_JSON.h>
#include <Wire.h>

// Replace with your network credentials
const char* ssid = "New Test";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
//AsyncEventSource events("/events");

// Create a WebSocket object
AsyncWebSocket ws("/ws");


// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // Update every 5 seconds

// Get Sensor Readings and return JSON object
String getSensorReadings(){
  readings["Nest"] = String(1);
  readings["Sensor"] =  String(1);
  readings["Temp"] = String(19.25);
  readings["Date"] =  ("Friday, May 10 2024");
  readings["Time"] =  ("16:24:30");
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
    Serial.println("SPIFFS mounted successfully");
  }
}

// Initialize SD card
void initSDCard() {
  if (!SD.begin()) {
    Serial.println("An error has occurred while mounting SD card");
  }
  else{
    Serial.println("SD card mounted successfully");
  }
}

/*
void readTemperatureData() {
  File dataFile = SD.open("/tempData1.csv", FILE_READ); // Open file for reading
  if (dataFile) {
    while (dataFile.available()) {
      String line = dataFile.readStringUntil('\n');
      int commaIndex = line.indexOf(',');
      if (commaIndex != -1) {
        // Parse temperature
        String temperatureStr = line.substring(0, commaIndex);
        float temperature = temperatureStr.toFloat();

        // Parse timestamp
        String timestampStr = line.substring(commaIndex + 1);
        unsigned long timestamp = strtoul(timestampStr.c_str(), NULL, 10);

        // Send temperature and timestamp to frontend
        JSONVar data;
        data["temperature"] = temperature;
        data["timestamp"] = timestamp;
        String jsonString = JSON.stringify(data);
        
        Serial.print(temperature);
        Serial.print(timestamp);
      }
    }
    dataFile.close();
  }
  else {
    Serial.println("Error opening file for reading");
  }
}
*/

void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    //data[len] = 0;
    //String message = (char*)data;
    // Check if the message is "getReadings"
    //if (strcmp((char*)data, "getReadings") == 0) {
      //if it is, send current sensor readings
      String sensorReadings = getSensorReadings();
     // Serial.print(sensorReadings);
      notifyClients(sensorReadings);
    //}
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

// Initialize WiFi in AP mode
void initWiFiAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("WiFi Access Point Successfully created - ESP32 IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  initSPIFFS();
  initSDCard(); // Initialize SD card
  initWiFiAP();
  //readTemperatureData();
  initWebSocket();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.serveStatic("/", SPIFFS, "/");

  // Start server
  server.begin();
}

void loop() {
if ((millis() - lastTime) > timerDelay) {
    String sensorReadings = getSensorReadings();
    //Serial.print(sensorReadings);
    notifyClients(sensorReadings);
  lastTime = millis();
  }
  ws.cleanupClients();
}
