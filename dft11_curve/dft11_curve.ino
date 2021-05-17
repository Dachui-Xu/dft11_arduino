#define BLINKER_WIFI
#define BLINKER_ALIGENIE_SENSOR
#define BLINKER_MIOT_SENSOR//小爱同学定义为传感器设备 
#include <Blinker.h>
#include <DHT.h>
 
char auth[] = "5672a8e8d143";
char ssid[] = "xudachui";
char pswd[] = "qncs8545";
 
BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");
 
#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
DHT dht(DHTPIN, DHTTYPE);
 
float humi_read = 0, temp_read = 0;
 
void heartbeat()
{
    HUMI.print(humi_read);
    TEMP.print(temp_read);
}

void dataStorage()
{
    Blinker.dataStorage("temp", temp_read);
    Blinker.dataStorage("humi", humi_read);
}

void aligenieQuery(int32_t queryCode)
{
    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.temp(temp_read);
            BlinkerAliGenie.humi(humi_read);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_TEMP_NUMBER :
            BlinkerAliGenie.temp(temp_read);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_HUMI_NUMBER :
            BlinkerAliGenie.humi(humi_read);
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.temp(20);
            BlinkerAliGenie.humi(20);
            BlinkerAliGenie.print();
            break;
    }
}
void miotQuery(int32_t queryCode)      //小爱同学语音命令反馈
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

            int humi_read_int=humi_read;     //去掉湿度浮点数
            BlinkerMIOT.humi(humi_read_int); //小爱反馈湿度属性
            BlinkerMIOT.temp(temp_read);     //小爱反馈温度属性
            BlinkerMIOT.print();//将以上属性发送给小爱，使得小爱可以接收到温湿度的数据

}
void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
 
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachHeartbeat(heartbeat);
    BlinkerAliGenie.attachQuery(aligenieQuery);
    Blinker.attachDataStorage(dataStorage);
    dht.begin();
}
 
void loop()
{
    Blinker.run();
 
    float h = dht.readHumidity();
    float t = dht.readTemperature();
 
    if (isnan(h) || isnan(t))
    {
        BLINKER_LOG("Failed to read from DHT sensor!");
    }
    else
    {
        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        humi_read = h;
        temp_read = t;
    }
 
   Blinker.delay(1000);
}
