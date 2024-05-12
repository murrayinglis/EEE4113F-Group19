#include <Wire.h> // For I2C communication
#include "RTClib.h" // Include the RTC library

RTC_DS3231 rtc; // Create an RTC object
DateTime newTime(2024, 5, 11, 12, 30, 0);

void setup() {
    Serial.begin(9600); // Initialize serial communication for debugging
    rtc.adjust(newTime);

    // Optional: Set the I2C pins if they're different from defaults
    // Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // Replace with your ESP32 pins if needed

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC!");
        while (1) delay(10);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, please set the time!");
        // Set the date and time here if necessary (refer to RTClib documentation)
    }
}

void loop() {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print("-");
    Serial.print(now.month(), DEC);
    Serial.print("-");
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.print(now.second(), DEC);
    Serial.println();

    delay(1000);
}
