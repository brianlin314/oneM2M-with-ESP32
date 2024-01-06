#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "USCC-2.4G";
const char *password = "NckuUSCC65607";

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void postToServer(String data) {
  HTTPClient http;

  // Replace the URL with the actual endpoint where you want to send the data
  String url = "http://192.168.0.73:5000/get_data";
  
  // Add headers if needed
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  // Send HTTP POST request with the data
  int httpResponseCode = http.POST(data);

  // Check for errors
  if (httpResponseCode > 0) {
    Serial.println("HTTP POST request success");
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("HTTP POST request failed, error code: ");
    Serial.println(httpResponseCode);
  }

  // Close connection
  http.end();
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // // Compute heat index in Fahrenheit (the default)
  // float hif = dht.computeHeatIndex(f, h);
  // // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);
  // // Your DHT sensor readings and calculations here

  // Example data to send
  String postData = "{\"humidity\":" + String(h) + ",\"temperature\":" + String(t) + "}";

  // Call the postToServer function to send data
  postToServer(postData);
}