/**********************************************************************
项目名称/Project          : 基于柔性传感器的图书馆智能座位管理系统
程序名称/Program name     : CenderCode
版本/Version             : V0.3.1 （测试版为发布版版本号前加0.）
团队/Team                : ThreeBugs
作者/Author              : EmberOfBag
日期/Date（YYYYMMDD）     : 20200609
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

串口发送允许标志位说明：WriteMark，使用D0引脚（GPIO5）作为主站串口发送标志，
在此代码中D1为输出
串口读取允许标志位说明：ReadMark，使用D2引脚（GPIO4）作为发送站串口读取标志，
在此代码中D2为输入
首先将D1置低,延迟后置高，可以给主站一个上升沿，触发主站的中断，发送一次数据，
主站发送前将D2置低，发送完将D2置高，给此发送站一个上升沿，触发此发送站中断，
读取数据，读取前将D1置低，读完置高，给主站一个上升沿，不断循环。
***********************************************************************/
#include <ESP8266WiFi.h>

const char *host = "39.99.152.88"; // 网络服务器地址  // Web address
const int httpPort = 80;           // http端口80     // HTTP port 80

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
// Set WiFi access information (please modify it according to your WiFi information)
const char *ssid = "Connect with 10 RMB";
const char *password = "zhanghao123";

//请求间隔时间
int RequestIntervalTime = 1000;

//请求标志时间
int RequestMarkTime = 0;

//构建存储数据的字符数组
volatile char SeatPressureData[15];
volatile char *RevBuf[20] = {0}; //存储分割后的，只要大于16就行

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

    //初始化串口写入/读取允许标志位
    pinMode(D1, OUTPUT); //WriteMark
    digitalWrite(D1, LOW);
    pinMode(D2, INPUT); //ReadMark
    int ReadMark = digitalRead(D2);

    //设置中断触发程序
    attachInterrupt(digitalPinToInterrupt(D2), ReadData, RISING);

    //延迟后将D1置高，给发送站第一个上升沿
    delay(20);
    digitalWrite(D1, HIGH);
}

void loop()
{
    if (RequestMarkTime >= RequestIntervalTime)
    {
        //    WifiClientRequest();
        RequestMarkTime = 0;
    }
    else
    {
        RequestMarkTime += 5;
    }
    delay(10);
}

//***********************构建接收到的数据***********************//
void ReadData()
{
    digitalWrite(D1, LOW);
    volatile int DataLength = Serial.available();
    volatile int num = 0;
    // 当串口接收到信息后
    Serial.print("\r\n============================================================\r\n");
    Serial.println("Received Serial Data:");
    volatile int i = 0;
    while (Serial.available())
    { // 当串口接收到信息后
        i++;
        SeatPressureData[i] = Serial.read();
    }
    for (i = 0; i < DataLength; i++)
    {                                      // 然后通过串口监视器输出 SeatPressureData
        Serial.print(SeatPressureData[i]); // 函数所读取的信息
    }
    Serial.println("");
    Serial.println("Finished Printing Recevied Data.");
    Serial.print("\r\n************************************************************\r\n");
    split(SeatPressureData, ",", RevBuf, &num);
    Serial.println("RevBuf Data:");
    for (int j = 0; j < num; j++)   // 然后通过串口监视器输出 RevBuf
    {
        char *Temp = RevBuf[j];                             
        Serial.print(sbzh); // 函数所读取的信息
        Serial.print(";");
    }
    Serial.print("\r\n");
    Serial.println("Finished Printing RevBuf Data.");
    Serial.print("\r\n============================================================\r\n");
    digitalWrite(D1, HIGH);
}

//*************************字符串分隔***************************//
/* 参数：  src 源字符串的首地址
**        separator  指定的分割字符
**        dest  接收子字符串的数组
**        num  分割后子字符串的个数
*/
void split(volatile char *src, const char *separator, volatile char **dest, volatile int *num)
{
    volatile char *pNext;
    volatile int count = 0;
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
    // 建立WiFi客户端对象，对象名称client
    WiFiClient client;

    // 构建座位数据字符串
    char ChairData[2048];
    sprintf(ChairData, "0=%c&1=%c&2=%c&3=%c&4=%c&5=%c&6=%c&7=%c&8=%c&9=%c&10=%c&11=%c&12=%c&13=%c&14=%c&15=%c", SeatPressureData[0], SeatPressureData[1], SeatPressureData[2], SeatPressureData[3], SeatPressureData[4], SeatPressureData[5], SeatPressureData[6], SeatPressureData[7], SeatPressureData[8], SeatPressureData[9], SeatPressureData[10], SeatPressureData[11], SeatPressureData[12], SeatPressureData[13], SeatPressureData[14], SeatPressureData[15]);
    //  ChairData = "0=0&1=0&2=0&3=0&4=0&5=0&6=0&7=0&8=0&9=0&10=0&11=0&12=0&13=0&14=0&15=0";

    // 将需要发送的数据信息放入客户端请求
    String url = "/chair_check.php?" + (String)ChairData;

    // 建立字符串，用于HTTP请求
    String httpRequest = String("GET /") + url + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: keep-alive\r\n" +
                         "Content-Type: application/x-www-form-urlencoded\r\n" +
                         "\r\n";

    // 通过串口输出连接服务器名称以便查阅连接服务器的网址
    Serial.print("------------------------------------------------------------\r\n");
    Serial.print("Connecting to ");
    Serial.print(host);

    // 连接服务器
    if (client.connect(host, httpPort))
    {
        Serial.println(" Success!"); // 连接成功后串口输出“Success”信息

        client.print(httpRequest);           // 向服务器发送HTTP请求
        Serial.println("Sending request: "); // 通过串口输出HTTP请求信息内容以便查阅
        Serial.println(httpRequest);

        // 通过串口输出网络服务器响应信息
        Serial.println("Web Server Response:");
        while (!client.available())
        {
        } //等待服务器返回数据
        while (client.available())
        { //client.connected() ||
            if (client.available())
            {
                String line = client.readStringUntil('\n');
                Serial.println(line);
            }
        }
        //    client.stop();                      // 断开与服务器的连接
        //    Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
        //    Serial.print(host);
    }
    else
    { // 如果连接不成功则通过串口输出“连接失败”信息
        Serial.println(" connection failed!");
        client.stop();
    }
    Serial.print("------------------------------------------------------------\r\n");
}
