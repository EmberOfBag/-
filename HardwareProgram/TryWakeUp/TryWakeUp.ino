void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);

  while(!Serial) { }

    Serial.println("5555555555");
    ESP.deepSleep(2e6); 
}

void loop() {
  delay(500);
}
