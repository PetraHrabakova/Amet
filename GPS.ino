void gpsWaitFix()
{
  uint8_t GPSchar;

  // Serial.println(F("Wait for updated GPS Fix"));
  GPSstarttimemS = millis();
  
  while (1)
  {
    if (gpsSerial.available() > 0)
    {
      GPSchar = gpsSerial.read();
      gps.encode(GPSchar);
    }

    if (gps.location.isUpdated() && gps.altitude.isUpdated())
    {
      GPSendtimemS = millis();
      break;
    }
  }
}


void displayGPSfix()
{
  GPSLat = String(gps.location.lat(), 6);
  GPSLon = String(gps.location.lng(), 6);

  Serial.print(F("Latitude: "));
  Serial.println(GPSLat);
  client.publish(latitude_topic, String(GPSLat).c_str());
  Serial.print(F("Longitude: "));
  Serial.println(GPSLon);
  client.publish(longitude_topic, String(GPSLon).c_str());
}
