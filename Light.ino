void light() {
  int brightness = analogRead(A0);
  Serial.print("Brightness: ");
  Serial.println(String(brightness).c_str());
  client.publish(brightness_topic, String(brightness).c_str(), true);

  if (brightness < 20) {
    
    Serial.println("going to deep sleep for 20 minutes, bye");
    ESP.deepSleep(1200e6); // 20 minutes 
  }

}
