// Smart Energy Meter - Basic Arduino Code

#include <SoftwareSerial.h>
#define sensor A0
#define relay 7

SoftwareSerial gsm(9, 10); // RX, TX for GSM Module

float voltage = 0.0;
float current = 0.0;
float power = 0.0;
int threshold = 300; // example value for theft detection

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); // Load ON by default
  delay(2000);
  sendSMS("Smart Energy Meter Initialized");
}

void loop() {
  int analogValue = analogRead(sensor);
  voltage = (analogValue * 5.0) / 1023.0; // Example scaling
  current = voltage / 0.185; // For ACS712 5A sensor
  power = voltage * current;

  Serial.print("Voltage: "); Serial.println(voltage);
  Serial.print("Current: "); Serial.println(current);
  Serial.print("Power: "); Serial.println(power);

  if (analogValue > threshold) {
    sendSMS("Alert: Power theft or overload detected!");
    digitalWrite(relay, LOW); // Turn off load
  }

  delay(5000);
}

void sendSMS(String msg) {
  gsm.println("AT+CMGF=1");  // Set SMS to text mode
  delay(1000);
  gsm.println("AT+CMGS=\"+91xxxxxxxxxx\""); // Replace with your number
  delay(1000);
  gsm.println(msg);
  delay(100);
  gsm.write(26); // Ctrl+Z to send
  delay(2000);
}
