
// Connect WiFi on the ESP8266
void connectwifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  //We want to be a client only
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  
  int waitTime=100;
  while (WiFi.status() != WL_CONNECTED) {
    delay(waitTime);
    waitTime = (waitTime==500)?500:waitTime+100;
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Disconnect Wifi
void disconnectwifi() {
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected");
}