void temp_humid() {
  
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  
  Serial.print("Temperature: ");
  Serial.println(String(temp).c_str());
  client.publish(temperature_topic, String(temp).c_str(), true);

  Serial.print("Humidity: ");
  Serial.println(String(hum).c_str());
  client.publish(humidity_topic, String(hum).c_str(), true);

}
