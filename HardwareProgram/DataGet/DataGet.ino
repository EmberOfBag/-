#include <ESP8266WiFi.h>
#include <espnow.h>

int force_value;

void setup() {
  Serial.begin(9600);//初始化串口，设置其波特率为9600
  
  WiFi.disconnect();//断开WIFI连接

}

void loop() {
  force_value = analogRead(A0); 
  Serial.println(force_value);
  delay(100);
}
