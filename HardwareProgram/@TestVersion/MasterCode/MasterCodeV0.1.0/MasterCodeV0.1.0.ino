/**********************************************************************
项目名称/Project          : 基于柔性传感器的图书馆智能座位管理系统
程序名称/Program name     : MasterCode
版本/Version             : V0.1.0 （测试版为发布版版本号前加0.）
团队/Team                : ThreeBugs
作者/Author              : EmberOfBag
日期/Date（YYYYMMDD）     : 20200521
程序目的/Purpose          : 
此程序为项目硬件部分从站的代码。实现NodeMCU与主站的通信，该程序实现从站（Master）向主站（Slave）发送数据。
NodeMCU以客户端模式（Station/STA）运行，用ESP-NOW实现数据通信。
此版本为测试版本，测试版本代码带有串口打印数据，发布版本将删除串口打印。

此代码为从站代码。此代码主要功能：
    - 检测压力传感器的压力数据
    - 通过ESP-Now发送压力数据给主站
-----------------------------------------------------------------------
ESP-NOW发送说明：
主站Mac地址： E8:DB:84:95:B6:96
***********************************************************************/
#include <ESP8266WiFi.h>
#include <espnow.h>

// 在此设置座位号，从1开始计算
// Set the chair_id here, starting from 1
const int CHAIR_ID = 1;

// 主站的mac地址
// MAC address of the master
uint8_t Esp_Controller[] = {0xE8, 0xDB, 0x84, 0x95, 0xB6, 0x96};

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
// Set WiFi access information (please modify it according to your WiFi information)
const char* ssid = "Connect with 10 RMB";

// 设置数据结构体
// Set the data structure
typedef struct msg {
  int chair_id;   
  double force_value;
} msg;
msg unit_data;

void setup() {
  delay(1000);
  
  Serial.begin(9600);   //初始化串口，设置其波特率为9600
  
  WiFi.mode(WIFI_AP);  //设置WIFI模式为STA
  WiFi.disconnect();    //断开WIFI连接

  //设置省电模式为NONE_SLEEP_T
  wifi_set_sleep_type(NONE_SLEEP_T);
  
  //初始化ESP-NOW
  // Init ESP-NOW
  if (esp_now_init() != 0)  
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("esp_now init success");
  }

  // 设定角色为 ESP_NOW_ROLE_CONTROLLER
  // Set ESP-NOW Role as ESP_NOW_ROLE_CONTROLLER
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  //注册回调
  // register callback function
  esp_now_register_send_cb(OnDataSent);
  
  //与控制模块配对
  // Register peer
  esp_now_add_peer(Esp_Controller, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0);
}

void loop() 
{
  //设置信道
//  esp_now_set_peer_channel(Esp_Controller,esp_now_get_peer_channel(Esp_Controller));
  // 设置要发送的数据
  unit_data.chair_id = CHAIR_ID;
//    value = analogRead(A0);                                 //注释掉的部分为将数值转化为实际电压值
//    voltage = ((float)map(value, 0, 1024, 0, 330))/100.00;  //注意,这两个变量未定义
  unit_data.force_value = analogRead(A0); 
  // Send message via ESP-NOW
  esp_now_send(Esp_Controller, (uint8_t *) &unit_data, sizeof(unit_data));
  delay(500);
}


//******************数据发送回调函数************************//
//Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) 
{
  //Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0)  //信息发送成功时
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

////***************获取wifi的信道*********************//
////get WiFi Channel
////扫描主站连接的ssid，获取其信道，用来给ESP-NOW通信提供信道
//int32_t getWiFiChannel(const char *ssid) {
//  if (int32_t n = WiFi.scanNetworks()) {          
//    for (uint8_t i=0; i<n; i++) {
//      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
//        Serial.println(WiFi.channel(i));
//        return WiFi.channel(i);
//      }
//    }
//  }
//  return 0;
//}
