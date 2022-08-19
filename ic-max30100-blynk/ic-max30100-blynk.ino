#define BLYNK_TEMPLATE_ID "TMPL49nb39SR"
#define BLYNK_DEVICE_NAME "max30100 bpm spO2"
#define BLYNK_AUTH_TOKEN "eQrWipPTH4HsmdE7tInThD7H05cP04Kd"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Shiguebarao Sala";
char pass[] = "nintendo1264";

// Creating a timer object
BlynkTimer timer;
// Create a PulseOximeter object
PulseOximeter pox;
// Create a MAX30100 object
// MAX30100 sensor;

float bpm, spo2;
int warnig_spo2 = 0;
int warnig_bpm = 0;
// uint16_t ir, red;

// Callback routine is executed when a pulse is detected
void onBeatDetected() {
  Serial.println("♥ Beat!");
  Blynk.virtualWrite(V0, bpm);
  Blynk.virtualWrite(V1, spo2);

  if (spo2 < 90) {
    warnig_spo2++;
  }
  if ( bpm < 50 || bpm > 80 ) {
    warnig_bpm++;
  }
  if (warnig_spo2 > 10)
  {
    Blynk.logEvent("spo2_warning");
    warnig_spo2 = 0;
  }
  if (warnig_bpm > 10)
  {
    Blynk.logEvent("bpm_warning");
    warnig_bpm = 0;
  }
  // Blynk.virtualWrite(V2, ir);
  // Blynk.virtualWrite(V3, red);


}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  bpm = pox.getHeartRate();
  spo2 = pox.getSpO2();
  Serial.print("Heart rate:");
  Serial.print(bpm);
  Serial.print("bpm / SpO2:");
  Serial.print(spo2);
  Serial.println("%");



  // while (sensor.getRawValues(&ir, &red)) {
  //  Serial.print(red);
  //  Serial.print(", ");
  //  Serial.println(ir);
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  // Setup a timer function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  Serial.print("Initializing pulse oximeter..");
  // Initialize sensor
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  // Register a callback routine
  pox.setOnBeatDetectedCallback(onBeatDetected);


  //  Serial.print("Initializing MAX30100..");
  //  // Initialize sensor
  //  if (!sensor.begin()) {
  //    Serial.println("FAILED");
  //    for (;;);
  //  } else {
  //    Serial.println("SUCCESS");
  //  }
  //  sensor.setMode(MAX30100_MODE_SPO2_HR);
  //  sensor.setLedsCurrent(MAX30100_LED_CURR_50MA, MAX30100_LED_CURR_27_1MA);
  //  sensor.setLedsPulseWidth(MAX30100_SPC_PW_1600US_16BITS);
  //  sensor.setSamplingRate(MAX30100_SAMPRATE_100HZ);
  //  sensor.setHighresModeEnabled(true);
}

void loop()
{
  Blynk.run();
  timer.run(); // runs the timer in the loop
  // Read from the sensor
  pox.update();
  // sensor.update();

}
