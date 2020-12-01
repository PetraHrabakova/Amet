#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define DHTPIN 14
#define wifi_ssid "ITEK 2nd"
#define wifi_password "Four_Sprints_F20v"

#define mqtt_server "test.mosquitto.org"
#define humidity_topic "BAAA/room1/Humidity"
#define temperature_topic "BAAA/room1/Temperature"
#define brightness_topic "BAAA/room1/Brightness"
#define GPS_topic "BAAA/room1/GPS"

#define DHTTYPE DHT22

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht = DHT(DHTPIN, DHTTYPE);
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();
  ss.begin(GPSBaud);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Hellooo")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void readGPS() {

  if (gps.location.isValid()) {
    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());

    char mqtt_payload[50] = "";
    snprintf (mqtt_payload, 50, "%lf;%lf", latitude, longitude);
    Serial.println(mqtt_payload);
    client.publish(GPS_topic, mqtt_payload);

    delay(2000);
  } else {
    Serial.println(F("INVALID"));
  }

}


void loop() {
  // To measure every 5 minutes, change it to 300000
  delay(2000);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      readGPS();

  // Read humidity, temperature, light intensity
  float hum = dht.readHumidity();

  float temp = dht.readTemperature();

  int lightsensorValue = analogRead(A0);
  float brightness = lightsensorValue * (5.0 / 10.23);

  Serial.print("Temperature: ");
  Serial.println(String(temp).c_str());
  client.publish(temperature_topic, String(temp).c_str(), true);

  Serial.print("Humidity: ");
  Serial.println(String(hum).c_str());
  client.publish(humidity_topic, String(hum).c_str(), true);

  Serial.print("Brightness: ");
  Serial.println(String(brightness).c_str());
  client.publish(brightness_topic, String(brightness).c_str(), true);

}
