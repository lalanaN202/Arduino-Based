#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
String apiKey = "XHPO8ZAE536IUB5X";     //  Enter your Write API key here
const char* server = "api.thingspeak.com";
const char *ssid =  "vivo 2018";     // Enter your WiFi Nameśś
const char *pass =  "ed87ab62203d"; // Enter your WiFi Password
#define DHTPIN 4
#define DHTTYPE    DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

const int moisturePin = A0; 
unsigned long interval = 10000;
unsigned long previousMillis = 0;
unsigned long interval1 = 1000;
unsigned long previousMillis1 = 0; 
float moisturePercentage;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  unsigned long currentMillis = millis();
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  dht.humidity().getEvent(&event);
  /*if (isnan(event.relative_humidity) || isnan(event.temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }*/
  moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) );
  if ((unsigned long)(currentMillis - previousMillis1) >= interval1) {
    Serial.print("Soil Moisture is  = ");
    Serial.print(moisturePercentage);
    Serial.println("%");
    previousMillis1 = millis();
  }
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    sendThingspeak();           //send data to thing speak
    previousMillis = millis();
    client.stop();
  }
}


void sendThingspeak() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  dht.humidity().getEvent(&event);
  if (client.connect(server, 80)) {
    String postStr = apiKey;              // add api key in the postStr string
    postStr += "&field3=";
    postStr += String(moisturePercentage);    // add mositure readin
    postStr += "&field2=";
    postStr += String(event.temperature);                 // add tempr readin
    postStr += "&field1=";
    postStr += String(event.relative_humidity);                  // add humidity readin
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());           //send lenght of the string
    client.print("\n\n");
    client.print(postStr);                      // send complete string
    Serial.print("Moisture Percentage: ");
    Serial.print(moisturePercentage);
    Serial.print("%. Temperature: ");
    Serial.print(event.temperature);
    Serial.print(" C, Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%. Sent to Thingspeak.");
  }
}
