/**********************************************************************
项目名称/Project          : 基于柔性传感器的图书馆智能座位管理系统
程序名称/Program name     : CenderCode
版本/Version             : V0.3.4 （测试版为发布版版本号前加0.）
团队/Team                : ThreeBugs
作者/Author              : EmberOfBag
日期/Date（YYYYMMDD）     : 20200623
程序目的/Purpose          : 
此程序为项目硬件部分主站的代码。实现NodeMCU与服务器的通信，该程序实现连接wifi的
NodeMCU（Sender）与主站（Slave）进行串口通信，数据并发送给服务器。
NodeMCU以客户端模式（Station/STA）运行。
此版本为测试版本，测试版本代码带有串口打印数据，发布版本将删除串口打印。

此代码为主站代码。此代码主要功能：
    - 通过串口通信接收主站（Slave）数据
    - 通过HTTP协议向服务器发送HTTP请求
    - HTTP请求中包含座位的压力数据，即从站数据。
-----------------------------------------------------------------------
其他说明：

http信息发送说明
39.99.152.88

串口说明：
串口发送中断说明：使用D1引脚（GPIO5）作为主站串口发送的外部中断。
首先此发送站读取数据前将D1置低，再置高，给主站一个上升沿，主站发送数据，发送站
延迟等待发送结束，延迟结束后读取数据，读完后再进入下一轮，不断循环。
***********************************************************************/
#include <ESP8266WiFi.h>

const char *host = "39.99.152.88"; // 网络服务器地址  // Web address
const int httpPort = 80;           // http端口80     // HTTP port 80

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
// Set WiFi access information (please modify it according to your WiFi information)
const char *ssid = "Connect with 10 RMB";
const char *password = "zhanghao123";

#define RequestIntervalTime 500 //发送一次http请求间隔时间
#define ReadDataTime 100        //等待读取完毕串口数据的时间

//请求标志时间
int RequestMarkTime = 0;
int RequestPreviousTime = 0;
int RequestCurrentTime = 0;

//构建存储数据的字符数组
char *RevBuf[20] = {0}; //存储分割后的，只要大于16就行
// 建立终止字符，作为串口数据结束标志
char TerminateChar = 'T';

void setup()
{
    // 延迟是必须的，要等待主站（Slave）初始化完成
    delay(5000);

    // 初始化串口设置
    // Initialize the serial setting
    Serial.begin(9600); //初始化串口，设置其波特率为9600
    Serial.println("初始化串口成功");

    // 设置ESP8266工作模式为无线终端模式
    // Set ESP8266 operating mode to STA mode
    WiFi.mode(WIFI_AP_STA); //设置WIFI模式为混合
    Serial.println("设置ESP8266工作模式为STA模式");

    // 设置省电模式为NONE_SLEEP_T，不需要省电
    wifi_set_sleep_type(NONE_SLEEP_T);

    // 开始连接wifi
    // Start connecting to WiFi
    WiFi.begin(ssid, password);
    Serial.print("开始连接wifi");

    // 等待WiFi连接,连接成功打印IP
    // Wait for WiFi to connect, print IP if connected successfully
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    //初始化D1（GPIO5），输出电平，作为主站的外部中断来源
    pinMode(D1, OUTPUT);
}

void loop()
{
    RequestCurrentTime = millis();
    RequestMarkTime = RequestCurrentTime - RequestPreviousTime + ReadDataTime; // 加ReadDataTime是因为在发送http请求之前在ReadData中会有延迟
    Serial.println("RequestMarkTime:");
    Serial.println(RequestMarkTime);
    if (RequestMarkTime >= RequestIntervalTime)
    {
        RequestPreviousTime = millis();
        WifiClientRequest();
        RequestMarkTime = 0;
    }
}

//***********************构建接收到的数据***********************//
void ReadData()
{
    char SeatPressureData[1024] = {0};
    int DataLength;
    int num;
    digitalWrite(D1, LOW);
    digitalWrite(D1, HIGH);
    delay(ReadDataTime); //延迟等待发送数据结束，延迟更好控制时间
    if (Serial.available())
    {
        DataLength = Serial.available();
        num = 0;
        Serial.readBytesUntil(TerminateChar, SeatPressureData, DataLength); //读取串口的数据到SeatPressureData中缓存，以TerminateChar,即T作为结束标志
        split(SeatPressureData, ",", RevBuf, &num);
        while (Serial.available())
        {
            Serial.read();
        }
    }
}

//*************************字符串分隔***************************//
/* 参数：  src 源字符串的首地址
**        separator  指定的分割字符
**        dest  接收子字符串的数组
**        num  分割后子字符串的个数
*/
void split(char *src, const char *separator, char **dest, int *num)
{
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0)
        return;
    if (separator == NULL || strlen(separator) == 0)
        return;
    pNext = strtok(src, separator);
    while (pNext != NULL)
    {
        *dest++ = pNext;
        ++count;
        pNext = strtok(NULL, separator);
    }
    *num = count;
}

//********************向服务器发送HTTP请求**********************//
void WifiClientRequest()
{
    // 串口获取数据
    ReadData();

    // 建立WiFi客户端对象，对象名称client
    WiFiClient client;

    // 构建座位数据字符串
    char ChairData[2048];

    sprintf(ChairData, "0=%s&1=%s&2=%s&3=%s&4=%s&5=%s&6=%s&7=%s&8=%s&9=%s&10=%s&11=%s&12=%s&13=%s&14=%s&15=0", RevBuf[0], RevBuf[1], RevBuf[2], RevBuf[3], RevBuf[4], RevBuf[5], RevBuf[6], RevBuf[7], RevBuf[8], RevBuf[9], RevBuf[10], RevBuf[11], RevBuf[12], RevBuf[13], RevBuf[14]);
    //  ChairData = "0=0&1=0&2=0&3=0&4=0&5=0&6=0&7=0&8=0&9=0&10=0&11=0&12=0&13=0&14=0&15=0";

    // 将需要发送的数据信息放入客户端请求
    String url = "/chair_check.php?" + (String)ChairData;

    // 建立字符串，用于HTTP请求
    String httpRequest = String("GET /") + url + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: keep-alive\r\n" +
                         "Content-Type: application/x-www-form-urlencoded\r\n" +
                         "\r\n";

    // 连接服务器
    if (client.connect(host, httpPort))
    {
        client.print(httpRequest);           // 向服务器发送HTTP请求


        while (!client.available())
        {
        } //等待服务器返回数据
        while (client.available())
        {
            //client.connected()
            if (client.available())
            {
                String line = client.readStringUntil('\n');
            }
        }
        //    client.stop();                      // 断开与服务器的连接
        //    Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
        //    Serial.print(host);
    }
    else // 如果连接不成功则通过串口输出“连接失败”信息
    {
        Serial.println(" connection failed!");
        client.stop();
    }
}
