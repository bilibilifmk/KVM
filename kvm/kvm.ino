/*
 
  KVMv1.3
  https://github.com/bilibilifmk/kvm
  
  需要依赖wifi_link_tool ：https://github.com/bilibilifmk/wifi_link_tool 
  

*/
//普通配网流程 1.链接WiFi 2.自动弹出配网或打开浏览器访问6.6.6.6 3.输入WiFi密码
//微信小程序配网 使用微信扫描二维码 打开小程序 点击通用设备配网  输入WiFi密码
#define FS_CONFIG
//激活文件系统模式配网
#include <wifi_link_tool.h>
//引入wifilinktool头文件


#define key 14
#define setpcio 16
#define usben 5 
#define hdmien 4 
#define jdq_io 13 
#define led 12 


int  powers = 1;
int zt = 1;
unsigned long previousMillis = 0;
const long interval = 200;
int ledValue = 0;
int leddirection = 0;
void  ICACHE_RAM_ATTR setpc();
void setup() {
  pinMode(key, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(key), setpc, FALLING);
  pinMode(setpcio, OUTPUT);
  pinMode(hdmien, OUTPUT);
  pinMode(usben, OUTPUT);
  pinMode(jdq_io, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(setpcio, LOW);
  digitalWrite(hdmien, HIGH);
  digitalWrite(usben, LOW);
  digitalWrite(jdq_io, HIGH);
  analogWrite(led, 50);
  Serial.begin(115200);
  //开启串口
  /////////////////////////////////////////////////////////基础设置//////////////////////////////////////////////////////////
  rstb = 0;
  //重置io
  stateled = 2;
  //指示灯io
  Hostname = "KVM";
  //设备名称 允许中文名称 不建议太长
  wxscan = true;
  //是否被小程序发现设备 开启意味该设备具有后台 true开启 false关闭
  Version = "0.1.3";
  //程序版本号
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //此处请勿添加代码
  load();
  ota();
  //启用ota服务功能
  //初始化WiFi link tool
  //在下面添加代码
  webServer.on("/in", in);
  webServer.on("/pc.png", png);
  webServer.on("/pc1", pc1);
  webServer.on("/pc2", pc2);
  webServer.on("/info", infos);
  webServer.on("/ota", otaindex);
  webServer.on("/win.jpg", win_jpg);
  webServer.on("/linux.jpg", linux_jpg);
  webServer.on("/mac.jpg", mac_jpg);
  webServer.on("/xbox.jpg", xbox_jpg);
  webServer.on("/ps.jpg", ps_jpg);

}

void in() {

  if (webServer.arg("in") == "1")
  {
    switch_pc(1);
    zt = 1;
    Serial.println("切换一");
    //串口输出
    webServer.send(200, "text/plain", "ojbk");
  }
  if (webServer.arg("in") == "2")
  {
    zt = 2;
    switch_pc(2);
    Serial.println("切换二");
    //串口输出
    webServer.send(200, "text/plain", "ojbk");
  }
  if (webServer.arg("in") == "off")
  {
    power(0);
    webServer.send(200, "text/plain", "ojbk");
  }
  if (webServer.arg("in") == "on")
  {
    power(1);
    webServer.send(200, "text/plain", "ojbk");
  }
}

void png() {
  File file = SPIFFS.open("/pc.png", "r");
  webServer.streamFile(file, "aimage/png");
  file.close();

}


void linux_jpg() {
  File file = SPIFFS.open("/linux.jpg", "r");
  webServer.streamFile(file, "aimage/jpg");
  file.close();
}
void win_jpg() {
  File file = SPIFFS.open("/win.jpg", "r");
  webServer.streamFile(file, "aimage/jpg");
  file.close();
}
void mac_jpg() {
  File file = SPIFFS.open("/mac.jpg", "r");
  webServer.streamFile(file, "aimage/jpg");
  file.close();
}
void xbox_jpg() {
  File file = SPIFFS.open("/xbox.jpg", "r");
  webServer.streamFile(file, "aimage/jpg");
  file.close();
}
void ps_jpg() {
  File file = SPIFFS.open("/ps.jpg", "r");
  webServer.streamFile(file, "aimage/jpg");
  file.close();
}




void pc1() {
  if (webServer.arg("ip") == "")
  {
    File file = SPIFFS.open("/pc1.txt", "r");
    if (file) {
      webServer.send(200, "text/plain", file.readString());
    }
    file.close();

  } else
  {
    String ip = webServer.arg("ip");
    SPIFFS.remove("/pc1.txt");
    File file = SPIFFS.open("/pc1.txt", "w");
    file.print(ip);
    webServer.send(200, "text/plain", "ojbk");
    file.close();
  }
}

void pc2() {
  if (webServer.arg("ip") == "")
  {
    File file = SPIFFS.open("/pc2.txt", "r");
    if (file) {
      webServer.send(200, "text/plain", file.readString());
    }
    file.close();

  } else
  {
    String ip = webServer.arg("ip");
    SPIFFS.remove("/pc2.txt");
    File file = SPIFFS.open("/pc2.txt", "w");
    file.print(ip);
    webServer.send(200, "text/plain", "ojbk");
    file.close();
  }
}

void infos() {
  File file = SPIFFS.open("/info.html", "r");
  webServer.streamFile(file, "text/html");
  file.close();

}

void otaindex() {
  File file = SPIFFS.open("/otaindex.html", "r");
  webServer.streamFile(file, "text/html");
  file.close();

}
void loop() {
  //请勿此处添加代码
  pant();
  //WiFi link tool 服务维持函数  请勿修改位置
  //请在下面添加自己的代码

  if (powers == 0) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (ledValue < 0) leddirection = 1;
      if (ledValue > 15) leddirection = 0;
      if (leddirection)
        analogWrite(led, ledValue++);
      else
        analogWrite(led, ledValue--);
    }
  }
  else
  {
    analogWrite(led, 50);
  }


}



void setpc() {
  //Serial.println("触发案件");
  digitalWrite(led, HIGH);
  //analogWrite(led, 255);
  bool res_state = true;
  unsigned int res_time = millis();
  while (millis() - res_time < 80) {

    if (digitalRead(key) != LOW) {
      res_state = false;
      //Serial.println("抖动");
      break;
    }
  }
  res_time = millis();
  while (digitalRead(key) == LOW)
  {

    if (millis() - res_time > 2000)
    {
      power(0);
      analogWrite(led, 0);
      Serial.println("关机");
      break;

    } else if (digitalRead(key) != LOW)
    {
      if (res_state == true) {
        if (powers != 1)
        {
          power(1);
        }
        else
        {
          if (zt == 1) {
            switch_pc(2);
            Serial.println("切换二");
            zt = 2;
            analogWrite(led, 0);
          } else {
            switch_pc(1);
            Serial.println("切换一");
            zt = 1;
            analogWrite(led, 0);
          }
        }
      }
      break;
    }



  }


  while (digitalRead(key) == LOW)
  {
    ESP.wdtFeed();
  }

  analogWrite(led, 50);

}





void switch_pc(int set)
{
  power(1);
  if (set == 1)
  {
    digitalWrite(setpcio, LOW);
  } else if (set == 2)
  {
    digitalWrite(setpcio, HIGH);
  }

}



void power (int switchs)
{
  if (switchs)
  {
    if (powers != 1)
    {
      digitalWrite(usben, LOW);
      digitalWrite(hdmien, HIGH);
      digitalWrite(jdq_io, HIGH);

      powers = 1;
      Serial.println("power on");
    }

  } else
  {
    digitalWrite(usben, HIGH);
    digitalWrite(hdmien, LOW);
    digitalWrite(jdq_io, LOW);
    powers = 0;
    Serial.println("power off");
  }

}
