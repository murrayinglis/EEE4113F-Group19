/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include <ESP32Time.h>

#define NEST_ID 1
unsigned long transmissionStartTime;
unsigned long transmissionEndTime;
unsigned long transmissionDuration;

// TEMP SENSOR
// GPIO where the DS18B20 is connected to
#define ONE_WIRE_BUS 13
void printAddress(DeviceAddress deviceAddress);
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
// Number of temperature devices found
int numberOfDevices;
// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress; 
float temp0;


// LORA
//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2


//TIME
//ESP32Time rtc;
ESP32Time rtc(3600);  // offset in seconds GMT+1

int counter = 0;

void setup() {
    //initialize Serial Monitor
    Serial.begin(9600);
    rtc.setTime(30, 24, 15, 17, 1, 2021); 


    while (!Serial);
    Serial.println("LoRa Sender");

    //setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);

    //replace the LoRa.begin(---E-) argument with your location's frequency 
    //433E6 for Asia
    //866E6 for Europe
    //915E6 for North America
    while (!LoRa.begin(433E6)) {
        Serial.println(".");
        delay(500);
    }
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");







    // Start the DS18B20 sensor
    sensors.begin();

    // Grab a count of devices on the wire
    numberOfDevices = sensors.getDeviceCount();

    // locate devices on the bus
    Serial.print("Locating devices...");
    Serial.print("Found ");
    Serial.print(numberOfDevices, DEC);
    Serial.println(" devices.");

    // Loop through each device, print out address
    for(int i=0;i<numberOfDevices; i++){
        // Search the wire for address
        if(sensors.getAddress(tempDeviceAddress, i)){
        Serial.print("Found device ");
        Serial.print(i, DEC);
        Serial.print(" with address: ");
        printAddress(tempDeviceAddress);
        Serial.println();
        } else {
        Serial.print("Found ghost device at ");
        Serial.print(i, DEC);
        Serial.print(" but could not detect address. Check power and cabling");
        }
    }
}

String msg = "";
String tm = "";
void loop() {

    //packet format: [Nest 0, Sensor 0, temp, time]

    // Get temp
    sensors.requestTemperatures(); 
    msg = "";
    for(int i = 0; i < numberOfDevices; i++) {
      temp0 = sensors.getTempCByIndex(i);
      tm = String(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
      msg = msg + "[Nest " + String(NEST_ID) + ", Sensor " + String(i+1) + ", Temp: " + String(temp0) + ", " + tm + "]\n";
      //Serial.print(msg);
      //Send LoRa packet to receiver
    } 
    transmissionStartTime = millis();
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
    transmissionEndTime = millis();
    transmissionDuration = transmissionEndTime - transmissionStartTime;
    Serial.print("Transmission Time (ms): ");
    Serial.println(transmissionDuration);
    // Calculate and print duty cycle
    unsigned long totalTransmissionTime = transmissionDuration;
    unsigned long totalTime = transmissionDuration + 60000; // Include delay between transmissions
    float dutyCycle = (float)totalTransmissionTime / totalTime * 100;
    Serial.print("Duty Cycle (%): ");
    Serial.println(dutyCycle);
    delay(60000);

}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
