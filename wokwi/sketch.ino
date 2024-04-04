#include "DHTesp.h"
#include <WiFi.h>
#include <ThingSpeak.h>
DHTesp dht;
WiFiClient  client;
const float GAMMA = 0.7;
const float RL10 = 50;
void setup() 
{
  Serial.begin(115200);
  Serial.println("GREENHOUSE MONITORING");
  pinMode(12, INPUT);
  dht.setup(15, DHTesp::DHT22);
  pinMode(17, OUTPUT);
  pinMode(16, INPUT);

  //connecting to wifi
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}
void loop() 
{
  //temperature and humidity sensor
  TempAndHumidity  data = dht.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");

  //light sensor
  if(digitalRead(12)==0)
  {
    Serial.println("on");
    ThingSpeak.setField(1, "day");
  }
  else{
    Serial.println("off");
    ThingSpeak.setField(1, "night");
  }
  //distance sensor
  digitalWrite(17, LOW);
  delayMicroseconds(2);
  digitalWrite(17, HIGH);
  delayMicroseconds(10);
  digitalWrite(17, LOW);

  long duration = pulseIn(16, HIGH);
  long distance = (duration * 0.034/2) + 1;
  Serial.println("Distance "+ String(distance));

  //write data to thingspeak
  ThingSpeak.setField(1, digitalRead(12));
  ThingSpeak.setField(2, data.temperature);
  ThingSpeak.setField(3, data.humidity);
  ThingSpeak.setField(4, distance);
  int statusCode = ThingSpeak.writeFields(2497276, "IFBMT6MAOUA3F9KF");

  if(statusCode == 200)
    Serial.println("Channel update successful");
  else
    Serial.println("Problem writing data error code: " + String(statusCode));
  delay(1000);
}
