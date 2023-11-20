/**********************************************************************
项目名称/Project          : 基于柔性传感器的图书馆智能座位管理系统
程序名称/Program name     : CenderCode
版本/Version             : V0.3.2 （测试版为发布版版本号前加0.）
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

//构建存储数据的字符数组

char *RevBuf[20] = {0};   //存储分割后的，只要大于16就行
char TerminateChar = 'T'; // 建立终止字符

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

    //初始化串口写入/读取允许标志位
    pinMode(D1, OUTPUT); //WriteMark
}

void loop()
{
    ReadData();
    delay(100);
}

//***********************构建接收到的数据***********************//
void ReadData()
{
    char SeatPressureData[1024] = {0};
    int DataLength;
    int num;
    digitalWrite(D1, LOW);
    digitalWrite(D1, HIGH);
    delay(100);
    if (Serial.available())
    {
        Serial.print("\r\n============================================================\r\n");
        DataLength = Serial.available();
        num = 0;
        // 当串口接收到信息后
        Serial.println("Received Serial Data:");
        Serial.readBytesUntil(TerminateChar, SeatPressureData, DataLength);
        for (int i = 0; i < DataLength; i++)
        {                                      // 然后通过串口监视器输出 SeatPressureData
            Serial.print(SeatPressureData[i]); // 函数所读取的信息
        }
        Serial.println("");
        Serial.println("Finished Printing Recevied Data.");
        Serial.print("\r\n************************************************************\r\n");

        split(SeatPressureData, ",", RevBuf, &num);
        Serial.println("RevBuf Data:");
        for (int j = 0; j < num; j++) // 然后通过串口监视器输出 RevBuf
        {
            Serial.print((char *)RevBuf[j]); // 函数所读取的信息
            Serial.print(";");
        }
        Serial.println("");
        Serial.println("Finished Printing RevBuf Data.");
        Serial.print("\r\n============================================================\r\n");
        while (Serial.available())
        { // 当串口接收到信息后
            Serial.read();
        }
        delay(100);
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
