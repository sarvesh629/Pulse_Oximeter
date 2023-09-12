#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LCD5110_Graph.h>

#define REPORTING_PERIOD_MS     1000

// Create a PulseOximeter object
PulseOximeter pox;
LCD5110 myGLCD(8,9,10,11,12);

// Time at which the last beat occurred
uint32_t tsLastReport = 0;
extern uint8_t SmallFont[];

// Callback routine is executed when a pulse is detected
void onBeatDetected() {
    Serial.println("Beat!");
}

void setup() {
    Serial.begin(9600);

    Serial.print("Initializing pulse oximeter..");

    // Initialize sensor
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);

myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
}

void loop() {
    // Read from the sensor
    pox.update();

    // Grab the updated heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
  myGLCD.clrScr();
  myGLCD.print((String) pox.getHeartRate(),CENTER,0);
  myGLCD.print((String) pox.getSpO2(),CENTER,20);

  myGLCD.update();

        tsLastReport = millis();
    }

}