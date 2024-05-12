#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <WiFi.h>

#define SD_CS_PIN 5 // Chip select pin for SD card module

File dataFile;

void writeData(String nest, String sensor, float temp, String date, String time) {
  dataFile = SD.open("/temperatureData1.csv", FILE_WRITE); // Ensure the file path starts with "/"
  if (dataFile) {
    dataFile.print("Nest: ");
    dataFile.print(nest);
    dataFile.print(", Sensor: ");
    dataFile.print(sensor);
    dataFile.print(", Temp: ");
    dataFile.print(temp);
    dataFile.print(", Date: ");
    dataFile.print(date);
    dataFile.print(", Time: ");
    dataFile.println(time);
    dataFile.close();
    Serial.println("Data written to temperatureData1.csv");
  } else {
    Serial.println("Error: Unable to write to file.");
  }
}

void readData() {
  dataFile = SD.open("/temperatureData1.csv"); // Ensure the file path starts with "/"
  if (dataFile) {
    Serial.println("Reading data from temperatureData1.csv:");
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  } else {
    Serial.println("Error: Unable to read from file.");
  }
}

void setup() {
  Serial.begin(115600);

  // Check if SD card is present
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Error: SD card initialization failed.");
    return;
  }
  Serial.println("SD card initialized.");

  // Check if the file exists, if not create it
  if (!SD.exists("/temperatureData1.csv")) { // Ensure the file path starts with "/"
    dataFile = SD.open("/temperatureData1.csv", FILE_WRITE); // Ensure the file path starts with "/"
    if (dataFile) {
      dataFile.println("Nest,Sensor,Temp,Date,Time");
      dataFile.close();
      Serial.println("New file created: temperatureData1.csv");
    } else {
      Serial.println("Error: Unable to create file.");
      return;
    }
  }

  // Write data to the file
  writeData("1", "1", 19.25, "Friday, May 10 2024", "16:24:30");

  // Read data from the file
  readData();
}

void loop() {
  // Nothing to do in the loop
}
