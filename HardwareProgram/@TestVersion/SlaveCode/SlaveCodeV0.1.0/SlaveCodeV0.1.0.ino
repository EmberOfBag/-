 /**********************************************************************
项目名称/Project          : 基于柔性传感器的图书馆智能座位管理系统
程序名称/Program name     : SlaveCode
版本/Version             : V0.1.0 （测试版为发布版版本号前加0.）
团队/Team                : ThreeBugs
作者/Author              : EmberOfBag
日期/Date（YYYYMMDD）     : 20200521
程序目的/Purpose          : 
此程序为项目硬件部分主站的代码。该程序实现主站（Slave）接收从站（Master）的数据
并发送给服务器。
NodeMCU以客户端模式（Station/STA）运行。
此版本为测试版本，测试版本代码带有串口打印数据，发布版本将删除串口打印。

此代码为主站代码。此代码主要功能：
    - 通过ESP-Now接收从站数据
    - 通过串口通信向连接wifi的NodeMCU（Sender）传输数据
-----------------------------------------------------------------------
无其他说明
***********************************************************************/
#include <ESP8266WiFi.h>
#include <espnow.h>

// 设置数据结构体
// Set the data structure
typedef struct msg {
  int chair_id;
  double force_value;
} msg;
msg unit_data;

//构建存储数据的数组及发送的字符串
uint16_t SeatPressureData[15];
uint16_t NormalizedData[15];
String SendData = "";

void setup() {
  // 初始化串口设置
  // Initialize the serial setting
  Serial.begin(9600);//初始化串口，设置其波特率为9600
  Serial.println("初始化串口成功");

  // 设置ESP8266工作模式为无线终端模式，并断开WIFI连接
  // Set ESP8266 operating mode to STA mode
  WiFi.mode(WIFI_STA);//设置WIFI模式为STA
  Serial.println("设置ESP8266工作模式为STA模式");
  WiFi.disconnect();

  //设置省电模式为NONE_SLEEP_T,主站接收数据,不需要也最好不休眠，否则影响ESP-NOW
  wifi_set_sleep_type(NONE_SLEEP_T);
  
  // 初始化ESP-NOW 
  // Init ESP-NOW
  if (esp_now_init() != 0)//初始化espnow
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("ESP-NOW init success!");
  }
  
  // 设定角色为 ESP_NOW_ROLE_CONTROLLER
  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  // 执行完这个函数，每次接收数据就会自动调用回调函数了
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  DataSend();
  delay(1000);
  }


//**********************数据接收回调函数***************************//
// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&unit_data, incomingData, sizeof(unit_data));
  SeatPressureData[unit_data.chair_id - 1] = unit_data.force_value;
}

//**********************串口通信发送数据函数************************//
// Serial communication to send data function
void DataSend(){
  Normalization();
  SendData = "";
  for(int k=0;k<16;k++){
    SendData += NormalizedData[k];
  }
  Serial.println(SendData);
}

//**********************发送数据归一化************************//
// Send data normalization
void Normalization(){
  for(int k=0;k<16;k++){
    if(SeatPressureData[k]>400){
      NormalizedData[k] = 1;
    }
    else{
      NormalizedData[k] = 0;
    }
  }
    
}
