// Indoor Air Quality Monitoring System using ESP32 and Blynk
// Author: Lakki Thejesh Kumar
// Project: CSP Project - B.E. ECE (AI & ML)
// Vel Tech Rangarajan Dr. Sagunthala R&D Institute

#define BLYNK_TEMPLATE_ID   "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitor"
#define BLYNK_AUTH_TOKEN    "YourAuthToken"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials - change to your WiFi
char ssid[] = "YourWiFiName";
char pass[] = "YourWiFiPassword";

// Pin definitions
const int MQ135_PIN = 34;  // Air quality sensor (analog)
const int MQ2_PIN   = 35;  // Gas sensor (analog)
const int LED_PIN   = 2;   // Onboard LED

// Thresholds
const int AIR_QUALITY_THRESHOLD = 300;
const int GAS_THRESHOLD         = 400;

// Blynk Virtual Pins
#define VPIN_AIR_QUALITY  V0
#define VPIN_GAS_LEVEL    V1
#define VPIN_STATUS       V2

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Send sensor data every 2 seconds
  timer.setInterval(2000L, sendSensorData);

  Serial.println("Air Quality Monitor Started!");
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensorData() {
  // Read sensor values
  int airQuality = analogRead(MQ135_PIN);
  int gasLevel   = analogRead(MQ2_PIN);

  // Map to 0-100 percentage
  int airPercent = map(airQuality, 0, 4095, 0, 100);
  int gasPercent = map(gasLevel,   0, 4095, 0, 100);

  // Print to Serial Monitor
  Serial.print("Air Quality: ");
  Serial.print(airPercent);
  Serial.print("%  |  Gas Level: ");
  Serial.print(gasPercent);
  Serial.println("%");

  // Send to Blynk app
  Blynk.virtualWrite(VPIN_AIR_QUALITY, airPercent);
  Blynk.virtualWrite(VPIN_GAS_LEVEL,   gasPercent);

  // Check thresholds and send alert
  if (airQuality > AIR_QUALITY_THRESHOLD || gasLevel > GAS_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
    Blynk.virtualWrite(VPIN_STATUS, "ALERT: Poor Air Quality!");
    Blynk.logEvent("air_quality_alert", "Poor air quality detected!");
    Serial.println("ALERT: Poor Air Quality Detected!");
  } else {
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(VPIN_STATUS, "Air Quality: Good");
  }
}
