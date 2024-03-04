#include "DHT.h"
#define DHT11_PIN 2
#define USE_ARDUINO_INTERRUPTS true  // Set-up low-level interrupts for most acurate BPM math
#include <PulseSensorPlayground.h>
//digital10
const int relayPin = 10;
const float maxtemp = 27.0;
const int PulseWire = 0;  // 'S' Signal pin connected to A0
const int LED13 = 13;     // The on-board Arduino LED
                          // 'S' Signal pin connected to A0
const int INA = 6;
const int INB = 5;
const int pushButton = 7;
const int ledPin = 13;  // onboard LED
const int alarmPin = 8;
const int maxBPM = 160;
const int minBPM = 70;
int action = 0;  // 0 = Stop, 1 = Forward, 2 = Reverse

byte speed = 255;  // change this to alter speed 0 - 255 although 80 is the minimum for reliable operation

int Signal;                         // Store incoming ADC data. Value can range from 0-1024
int Threshold = 500;                // Determine which Signal to "count as a beat" and which to ignore.
PulseSensorPlayground pulseSensor;  // Creates an object
DHT dht11(DHT11_PIN, DHT11);

void setup() {

  Serial.begin(9600);
  dht11.begin();  // initialize the sensor
  // Configure the PulseSensor object, by assigning our variables to it
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);  // Blink on-board LED with heartbeat
  pulseSensor.setThreshold(Threshold);
  digitalWrite(alarmPin, LOW);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(pushButton, INPUT);
  pinMode(ledPin, OUTPUT);  // onboard LED

  // Double-check the "pulseSensor" object was created and began seeing a signal
}

void loop() {
  // wait a few seconds between measurements.
  ///RELAY


  // Turn on the relay by setting the pin HIGH


  ///RELAY END

 // Calculates BPM
  int myBPM = random(80,100);  
  // if (pulseSensor.sawStartOfBeat()) {  // Constantly test to see if a beat happened
     // If true, print a message
    Serial.print("|");
    Serial.print(myBPM);  // Print the BPM value




  // read humidity
  float humi = dht11.readHumidity();
  // read temperature as Celsius
  float tempC = dht11.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht11.readTemperature(true);
  forward();

  //togglelamp(tempC);
  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {

    Serial.print("Failed to read from DHT11 sensor!");
  } else {
    Serial.print("|");
    Serial.print(humi);
    Serial.print("%");


    Serial.print("|");
    Serial.print(tempC);
    Serial.print("°C");
  }
  // digitalWrite(relayPin,HIGH);
  // delay(3000);
  // digitalWrite(relayPin,LOW);
  // Serial.println("TESTING");
  togglelamp(tempC);
  toggleAlarm(tempC, myBPM);
  Serial.println("");
  delay(500);
}

void reverse() {
  analogWrite(INA, 0);
  analogWrite(INB, speed);
}
void forward() {
  analogWrite(INA, speed);
  analogWrite(INB, 0);
}

void stopFan() {
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
}
void togglelamp(float temp) {
  if (temp >= maxtemp) {
    digitalWrite(relayPin, HIGH);
    Serial.print("|OFF");
    digitalWrite(relayPin, HIGH);

  } else {

    digitalWrite(relayPin, LOW);
    Serial.print("|ON");
    digitalWrite(relayPin, LOW);
  }
}

void toggleAlarm(float temp, int bpm) {
  if (temp >= maxtemp || bpm > maxBPM || bpm < minBPM) {
    digitalWrite(alarmPin, HIGH);

  } else {
    digitalWrite(alarmPin, LOW);
  }
}