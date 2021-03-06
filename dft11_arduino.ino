#define BLINKER_WIFI //定义wifi模块
#define BLINKER_MIOT_SENSOR//小爱同学定义为传感器设备
#include <Blinker.h>//包含Blinker头文件
#include <DHT.h>//包含DHT头文件

#define DHTPIN 2      //定义DHT11模块连接管脚io2

#define DHTTYPE DHT11   // 使用温度湿度模块的类型为DHT11

char auth[] = "d4882f8d5170";  //你的设备key
char ssid[] = "xudachui";  //你的wifi 名称
char pswd[] = "qncs8545";  //你的wifi 密码

//新建数据类型组件对象，作用：将数据传输到手机blinker app
BlinkerNumber HUMI("humi");    //定义湿度数据键名
BlinkerNumber TEMP("temp");    //定义温度数据键名

DHT dht(DHTPIN, DHTTYPE);    //生成DHT对象，参数是引脚和DHT的类型

float humi_read = 0, temp_read = 0;//定义浮点型全局变量 储存传感器读取的温湿度数据

void heartbeat()
{
    HUMI.print(humi_read);        //给blinkerapp回传湿度数据
    TEMP.print(temp_read);        //给blinkerapp回传温度数据
}

void miotQuery(int32_t queryCode)      //小爱同学语音命令反馈
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

            int humi_read_int=humi_read;     //去掉湿度浮点数
            BlinkerMIOT.humi(humi_read_int); //小爱反馈湿度属性
            BlinkerMIOT.temp(temp_read);     //小爱反馈温度属性
            BlinkerMIOT.print();//将以上属性发送给小爱，使得小爱可以接收到温湿度的数据

}


void setup()//将设置代码放在此处，运行一次；
{
    //初始化端口
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();

    Blinker.begin(auth, ssid, pswd); // // 初始化blinker
    Blinker.attachHeartbeat(heartbeat);//将传感器获取的数据传给blinker app上
    dht.begin();//初始化DHT传感器
    //在回调函数中反馈该控制状态
    BlinkerMIOT.attachQuery(miotQuery);//每次呼出小爱同学，就会调用miotQuery()函数
}
//通过循环不断读取温湿度传感器获取的数据
void loop() //把主代码放在这里，重复运行：
{
    Blinker.run();//运行Blinker

    float h = dht.readHumidity();//读取DHT11传感器的湿度 并赋值给h
    float t = dht.readTemperature();//读取传感器的温度   并赋值给t
    if (isnan(h) || isnan(t))//判断是否成功读取到温湿度数据
    {
        BLINKER_LOG("Failed to read from DHT sensor!");//读取温湿度失败！
    }
    else//成功读取到数据
    {   
        //打印
        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");

        humi_read = h;//将读取到的湿度赋值给全局变量humi_read
        temp_read = t;//将读取到的温度赋值给全局变量temp_read
    }
    Blinker.delay(2000);//延时函数
}
