/**********************************************************************
项目名称/Project          : 基于柔性传感器的图书馆智能座位管理系统
程序名称/Program name     : MasterClient
团队/Team                : ThreeBugs
作者/Author              : EmberOfBag
日期/Date（YYYYMMDD）     : 20200518
程序目的/Purpose          : 
实现NodeMCU与服务器的通信，该程序为主站（Master）程序的与服务器的通信部分。
NodeMCU以客户端模式（Station/STA）运行。

此代码为客户端代码。此代码主要功能：
    - 通过HTTP协议向服务器发送HTTP请求
    - HTTP请求中包含座位的压力数据。
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
-----------------------------------------------------------------------
http信息发送说明
39.99.152.88
***********************************************************************/
#include <ESP8266WiFi.h>

const char* host = "39.99.152.88"; // 网络服务器地址
const int httpPort = 80;              // http端口80

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "Connect with 10 RMB";
const char* password = "zhanghao123";

void setup() {
  delay(1000);
  //初始化串口设置
  Serial.begin(9600);
  Serial.println("初始化串口成功");
 
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
  Serial.println("设置ESP8266工作模式为STA模式");
 
  //开始连接wifi
  WiFi.begin(ssid, password);
  Serial.print("开始连接wifi");
  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  
}

void loop(){
  Serial.print("========================================\n");
  delay(1000);
  wifiClientRequest();
}

// 向服务器发送HTTP请求
void wifiClientRequest(){
  // 建立WiFi客户端对象，对象名称client
  WiFiClient client;    

  // 构建座位数据字符串
  String ChairData;
  //  sprintf(ChairData, "0=%d&1=%d&2=%d&3=%d&4=%d&5=%d&6=%d&7=%d&8=%d&9=%d&10=%d&11=%d&12=%d&13=%d&14=%d&15=%d", Seats[0], Seats[1], Seats[2], Seats[3], Seats[4], Seats[5], Seats[6], Seats[7], Seats[8],Seats[9], Seats[10], Seats[11], Seats[12], Seats[13], Seats[14], Seats[15]);
  ChairData = "0=0&1=0&2=0&3=0&4=0&5=0&6=0&7=0&8=0&9=0&10=0&11=0&12=0&13=0&14=0&15=0";
  
  // 将需要发送的数据信息放入客户端请求
  String url = "/chair_check.php?" + ChairData;
  
  // 建立字符串，用于HTTP请求
  String httpRequest =  String("GET /") + url + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: keep-alive\r\n" +
                        "Content-Type: application/x-www-form-urlencoded\r\n" +
                        "\r\n";
  
  // 通过串口输出连接服务器名称以便查阅连接服务器的网址                      
  Serial.print("Connecting to "); 
  Serial.print(host); 

  // 连接服务器
  if (client.connect(host, httpPort)){ 
    Serial.println(" Success!");        // 连接成功后串口输出“Success”信息
    
    client.print(httpRequest);          // 向服务器发送HTTP请求
    Serial.println("Sending request: ");// 通过串口输出HTTP请求信息内容以便查阅
    Serial.println(httpRequest);     
    
    // 通过串口输出网络服务器响应信息
    Serial.println("Web Server Response:");        
    while (!client.available()){}     //等待服务器返回数据 
    while (client.available()){  //client.connected() || 
      if (client.available()){
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
//    client.stop();                      // 断开与服务器的连接
//    Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
//    Serial.print(host);
    Serial.print("========================================\r\n");
  } else {    // 如果连接不成功则通过串口输出“连接失败”信息
    Serial.println(" connection failed!");
    client.stop();
  }  
}
