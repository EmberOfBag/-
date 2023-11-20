#include <ESP8266WiFi.h>

String SendData = "0000,1111,2222,3333,4444,5555,6666,7777,8888,9999,1010,1111,1212,1313,1414,1515T";

void setup()
{
    Serial.begin(9600); //设置串口波特率为115200
    Serial1.begin(9600);
    Serial.println("初始化串口成功");
    WiFi.mode(WIFI_STA); //设置WIFI模式为STA
    Serial.println("设置ESP8266工作模式为STA模式");
    WiFi.disconnect();
    wifi_set_sleep_type(NONE_SLEEP_T);

    //初始化串口发送/读取允许标志位
    pinMode(D1, INPUT); //WriteMark
    int SendMark = digitalRead(D1);

    //设置中断触发程序
    attachInterrupt(digitalPinToInterrupt(D1), DataSend, RISING);
}

void loop()
{
}

//**********************串口通信发送数据函数************************//
// Serial communication to send data function
ICACHE_RAM_ATTR void DataSend()
{
    Serial.println(SendData);
    Serial1.print(SendData);
}
