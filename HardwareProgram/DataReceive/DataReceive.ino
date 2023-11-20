#include <ESP8266WiFi.h>
#include <espnow.h>


// 设置数据结构体
typedef struct msg {
  int chair_id;
  double force_value;
} msg;
msg unit_data;



void setup() {
  Serial.begin(9600);//初始化串口，设置其波特率为9600
  WiFi.mode(WIFI_STA);//设置WIFI模式为STA
  WiFi.disconnect();//断开WIFI连接
  
  // Init ESP-NOW
  if (esp_now_init() != 0)//初始化espnow
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("esp_now init success");
  }

  // Set ESP-NOW Role
  //双向通信时需要设定角色为 ESP_NOW_ROLE_CONTROLLER
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  // Register for a callback function that will be called when data is received
  //执行完这个函数，每次接收数据就会自动调用回调函数了
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() 
{
  delay(1000);
  Serial.println("debug!");
}

// 数据接收回调函数
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&unit_data, incomingData, sizeof(unit_data));
  Serial.print("seat_id: ");
  Serial.println(unit_data.chair_id);
  Serial.print("force_value: ");
  Serial.println(unit_data.force_value);
  Serial.println();
}
