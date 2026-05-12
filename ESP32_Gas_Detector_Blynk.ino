#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Gas Detector Esp32"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

#define MQ5_PIN 34
#define MQ7_PIN 35

const int MQ5_WARNING_DIFF = 150;
const int MQ5_DANGER_DIFF = 350;

const int MQ7_WARNING_DIFF = 120;
const int MQ7_DANGER_DIFF = 300;

int mq5Baseline = 0;
int mq7Baseline = 0;

BlynkTimer timer;

int readAverage(int pin) {
  long sum = 0;

  for (int i = 0; i < 30; i++) {
    sum += analogRead(pin);
    delay(5);
  }

  return sum / 30;
}

int calculateBaseline(int pin) {
  long sum = 0;

  for (int i = 0; i < 60; i++) {
    sum += readAverage(pin);
    delay(200);
  }

  return sum / 60;
}

String getStatus(int diff, int warningDiff, int dangerDiff) {
  if (diff < 0) {
    diff = 0;
  }

  if (diff < warningDiff) {
    return "NORMAL";
  } else if (diff < dangerDiff) {
    return "WARNING";
  } else {
    return "DANGER";
  }
}

int getPercent(int diff, int dangerDiff) {
  if (diff < 0) {
    diff = 0;
  }

  int percent = map(diff, 0, dangerDiff, 0, 100);

  if (percent < 0) {
    percent = 0;
  }

  if (percent > 100) {
    percent = 100;
  }

  return percent;
}

String getSystemStatus(String mq5Status, String mq7Status) {
  if (mq5Status == "DANGER" || mq7Status == "DANGER") {
    return "DANGER";
  }

  if (mq5Status == "WARNING" || mq7Status == "WARNING") {
    return "WARNING";
  }

  return "NORMAL";
}

void sendSensorData() {
  int mq5Raw = readAverage(MQ5_PIN);
  int mq7Raw = readAverage(MQ7_PIN);

  int mq5Diff = mq5Raw - mq5Baseline;
  int mq7Diff = mq7Raw - mq7Baseline;

  int mq5Percent = getPercent(mq5Diff, MQ5_DANGER_DIFF);
  int mq7Percent = getPercent(mq7Diff, MQ7_DANGER_DIFF);

  String mq5Status = getStatus(mq5Diff, MQ5_WARNING_DIFF, MQ5_DANGER_DIFF);
  String mq7Status = getStatus(mq7Diff, MQ7_WARNING_DIFF, MQ7_DANGER_DIFF);

  String systemStatus = getSystemStatus(mq5Status, mq7Status);

  Blynk.virtualWrite(V0, mq5Raw);
  Blynk.virtualWrite(V1, mq7Raw);
  Blynk.virtualWrite(V2, mq5Percent);
  Blynk.virtualWrite(V3, mq7Percent);
  Blynk.virtualWrite(V4, systemStatus);
  Blynk.virtualWrite(V5, mq5Status);
  Blynk.virtualWrite(V6, mq7Status);

  Serial.print("SYSTEM: ");
  Serial.print(systemStatus);

  Serial.print(" || MQ5 Raw: ");
  Serial.print(mq5Raw);
  Serial.print(" | Base: ");
  Serial.print(mq5Baseline);
  Serial.print(" | Diff: ");
  Serial.print(mq5Diff);
  Serial.print(" | Level: ");
  Serial.print(mq5Percent);
  Serial.print("% | Status: ");
  Serial.print(mq5Status);

  Serial.print(" || MQ7 Raw: ");
  Serial.print(mq7Raw);
  Serial.print(" | Base: ");
  Serial.print(mq7Baseline);
  Serial.print(" | Diff: ");
  Serial.print(mq7Diff);
  Serial.print(" | Level: ");
  Serial.print(mq7Percent);
  Serial.print("% | Status: ");
  Serial.println(mq7Status);
}

void setup() {
  Serial.begin(9600);
  delay(2000);

  analogReadResolution(12);
  analogSetPinAttenuation(MQ5_PIN, ADC_11db);
  analogSetPinAttenuation(MQ7_PIN, ADC_11db);

  Serial.println();
  Serial.println("======================================");
  Serial.println(" ESP32 Gas Detector - MQ5 + MQ7");
  Serial.println(" Blynk + WiFi Version");
  Serial.println("======================================");
  Serial.println();

  Serial.println("Keep sensors in clean normal air.");
  Serial.println("Calculating baseline...");
  Serial.println();

  mq5Baseline = calculateBaseline(MQ5_PIN);
  mq7Baseline = calculateBaseline(MQ7_PIN);

  Serial.println("Baseline finished.");
  Serial.print("MQ5 Baseline = ");
  Serial.println(mq5Baseline);
  Serial.print("MQ7 Baseline = ");
  Serial.println(mq7Baseline);

  Serial.println();
  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendSensorData);

  Serial.println("System started.");
  Serial.println("======================================");
}

void loop() {
  Blynk.run();
  timer.run();
}
