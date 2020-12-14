#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// DHT setup
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht = DHT(DHTPIN, DHTTYPE);

// GPS setup
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);
String GPSLat;                     // Latitude from GPS
String GPSLon;                     // Longitude from GPS
uint32_t GPSstarttimemS;          // Time in mS when GPS is scanned fo fix
uint32_t GPSendtimemS;            // Time in mS when GPS got a fix

// Wifi setup
#define wifi_ssid "Petra"
#define wifi_password "Hrabka1998"

// MQTT setup
#define mqtt_server "172.20.10.4"
#define mqtt_user "amet"
#define mqtt_password "project3"
#define humidity_topic "BAAA/room1/Humidity"
#define temperature_topic "BAAA/room1/Temperature"
#define brightness_topic "BAAA/room1/Brightness"
#define latitude_topic "BAAA/room1/Latitude"
#define longitude_topic "BAAA/room1/Longitude"

WiFiClient espClient;
PubSubClient client(espClient);



void setup() {
  Serial.begin(115200);
  connectWifi();
  client.setServer(mqtt_server, 1883);
  gpsSerial.begin(GPSBaud);
  dht.begin();
}

void connectWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT()
{
  while (!client.connected())
  {
    String clientId = "Amet";
    clientId += String(random(0xf), HEX);
    Serial.printf("MQTT connecting as client %s...\n", clientId.c_str());
    if (client.connect(clientId.c_str(),mqtt_user,mqtt_password))
    {
      Serial.println("MQTT connected");
    } else
    {
      Serial.printf("MQTT failed, state %s, retrying...\n", client.state());
      delay(2500);
    }
  }
}


void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  // GPS
  gpsWaitFix();
  displayGPSfix();

  // Temperature and humidity
  temp_humid();

  // Light sensor
  light();

  delay(2000);
  Serial.println("going to deep sleep, bye");
  ESP.deepSleep(300e6); // 5 minutes seconds
}
