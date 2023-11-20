/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : csd_client
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200228
程序目的/Purpose          : 
演示如何实现两个NodeMCU间通过WiFi进行通讯。两台NodeMCU一个以服务器模式运行，
一个以客户端模式运行。

此代码为客户端代码。此代码主要功能：
    - 通过HTTP协议向服务器发送HTTP请求
    - HTTP请求中包含客户端按键开关引脚状态和程序测试数据
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
-----------------------------------------------------------------------
http信息发送说明
192.168.0.123/update?float=1.5&int=2&button=0
***********************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>   // 使用WiFiMulti库 

#define buttonPin D3            // 按钮引脚D3

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'

int Seats[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //存储座位信息

const char* host = "http://39.99.152.88/chair_check.php";    // 即将连接服务器网址/IP
const int httpPort = 22;               // 即将连接服务器端口

void setup(void){
  Serial.begin(9600);                  // 启动串口通讯
  Serial.println("");
  
  wifiMulti.addAP("Connect with 10 RMB", "zhanghao123"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  
  while (wifiMulti.run() != WL_CONNECTED) { // 尝试进行wifi连接。
    delay(250);
    Serial.print('.');
  }

  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP
}

void loop(void){

//获取座位信息

 // 发送请求
 wifiClientRequest();
 delay(1000);
}

void wifiClientRequest(){
  WiFiClient client;  

  // 构建座位数据字符串
  String ChairData;
//  sprintf(ChairData, "0=%d&1=%d&2=%d&3=%d&4=%d&5=%d&6=%d&7=%d&8=%d&9=%d&10=%d&11=%d&12=%d&13=%d&14=%d&15=%d", Seats[0], Seats[1], Seats[2], Seats[3], Seats[4], Seats[5], Seats[6], Seats[7], Seats[8],Seats[9], Seats[10], Seats[11], Seats[12], Seats[13], Seats[14], Seats[15]);
  ChairData = "0=0&1=0&2=0&3=0&4=0&5=0&6=0&7=0&8=0&9=0&10=0&11=0&12=0&13=0&14=0&15=0";
  
  // 将需要发送的数据信息放入客户端请求
  String url = "/chair_check.php?" + ChairData;
                         
  // 建立字符串，用于HTTP请求
  String httpRequest =  String("POST ") + url + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n" +
                        "\r\n";
                        
  Serial.print("Connecting to "); 
  Serial.print(host); 
  
  
 
    
    client.print(httpRequest);          // 向服务器发送HTTP请求
    Serial.println("Sending request: ");// 通过串口输出HTTP请求信息内容以便查阅
    Serial.println(httpRequest);        
  
  }
  
  client.stop();                         
}
