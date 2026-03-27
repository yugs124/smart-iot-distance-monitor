#define BLYNK_TEMPLATE_ID "TMPL3pMd5u1JW"
#define BLYNK_TEMPLATE_NAME "UV Sensor ioT"
#define BLYNK_AUTH_TOKEN "AuEF_o5S2AmgHtp197ZQksgySyXi37VH"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Jio 5g";
char pass[] = "12345678";

#define TRIG_PIN 15
#define ECHO_PIN 18
#define LED_PIN 21

long duration;
float distance;

BlynkTimer timer;

void sendSensorData() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo
  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    Serial.println("No object detected / Sensor timeout");
    digitalWrite(LED_PIN, LOW);

    Blynk.virtualWrite(V0, 0);   // Distance
    Blynk.virtualWrite(V1, 0);   // LED status
  } else {
    distance = duration * 0.0343 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    Blynk.virtualWrite(V0, distance);

    if (distance < 10) {
      digitalWrite(LED_PIN, HIGH);
      Blynk.virtualWrite(V1, 1);
    } else {
      digitalWrite(LED_PIN, LOW);
      Blynk.virtualWrite(V1, 0);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, sendSensorData);

  Serial.println("ESP32 Ultrasonic Sensor + Blynk Ready");
}

void loop() {
  Blynk.run();
  timer.run();
}