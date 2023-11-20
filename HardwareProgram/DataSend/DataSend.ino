#include <ESP8266WiFi.h>
#include <espnow.h>

//chair_id
const String CHAIR_ID = "1";

//主机模块的mac地址
uint8_t esp_controller[] = {0xE8, 0xDB, 0x84, 0x95, 0xB6, 0x96};

// 设置数据结构体
typedef struct msg {
  int chair_id;
  double force_value;
} msg;
msg unit_data;

//Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) 
{
  //Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0)//信息发送成功时
  {
    //打印接收方的mac地址
    Serial.print("deliver data to:  ");
    for(int i = 0; i < 5; ++i)
    {
      Serial.printf("%x:", *(mac_addr + i));
    }
    Serial.printf("%x", *(mac_addr + 5));
    Serial.println(" ");
    //Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

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

  // Set ESP-NOW Role as ESP_NOW_ROLE_SLAVE
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

  // register callback function
  //注册回调
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  //与控制模块配对
  esp_now_add_peer(esp_controller, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() 
{
    // 设置要发送的数据
    unit_data.chair_id = CHAIR_ID;
    unit_data.force_value = analogRead(A0); 
    // Send message via ESP-NOW
    esp_now_send(esp_controller, (uint8_t *) &unit_data, sizeof(unit_data));
    delay(500);
}
