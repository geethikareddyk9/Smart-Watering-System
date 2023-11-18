// Include the necessary libraries
#include <Wire.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3PHBwWs1w"
#define BLYNK_TEMPLATE_NAME "SoilMoisture"
#define BLYNK_AUTH_TOKEN "1zQJNLlFwioS8yAzo0LMSPknCwKeztcH"

// Set up the Blynk app credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Hexathon23";
char pass[] = "H3x@Th0n2@2#";

// Define the pins used for the sensors and relay module
#define DHTPIN 4
#define SOIL_MOISTURE_PIN 34
#define RELAY_PIN 26

// Initialize the sensors
DHT dht(DHTPIN, DHT11);

// Define variables for the sensor readings
float temperature = 0;
float humidity = 0;
float soil_moisture = 0;

// Initialize the Blynk app
void setup_blynk() {
  Blynk.begin(auth, ssid, pass);
}

// Read the sensor values and update the variables
void read_sensors() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  soil_moisture = (analogRead(SOIL_MOISTURE_PIN));
  soil_moisture = 100 - (soil_moisture/4095.00)*100;
}

// Control the relay based on the soil moisture level
void control_relay() {
  if (soil_moisture >= 45) { //45 is the threshold moisture percentage for a prototype.
    digitalWrite(RELAY_PIN, LOW);
    Blynk.virtualWrite(V4, 0);
  }
  else{
    digitalWrite(RELAY_PIN, HIGH);
    Blynk.virtualWrite(V4, 1 );
  }
}

// Send the sensor data to the Blynk app
void send_data_to_blynk() {
  Blynk.virtualWrite(V1, soil_moisture);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);
}

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  digitalWrite(RELAY_PIN, LOW);

  dht.begin();

  setup_blynk();
}

void loop() {
  read_sensors();
  control_relay();
  send_data_to_blynk();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Soil Moisture: ");
  Serial.print(soil_moisture);
  Serial.println(" %");

  Blynk.run();
delay(2000);
}
