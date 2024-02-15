#include <SoftwareSerial.h>
SoftwareSerial mySerial(12,13);
#include <Wire.h>
int sensorPin = A4;
int gas=A5;
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <Servo.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define tr1 A0
#define ec1 A1
#define tr2 A3 
#define ec2 A2
int kk=0;
Servo feed;
int ir=11;
int mp1=6;
int mp2=7;
int m1=2;
int m2=3;
int m3=4;
int m4=5;
void setup()
{   
    Wire.begin();
    Serial.begin(9600);
    mySerial.begin(115200);
    sensors.begin();
    pinMode(gas,INPUT);
    pinMode(sensorPin,INPUT);
    pinMode(ec1,INPUT);
    pinMode(ec2,INPUT);
    pinMode(tr1,OUTPUT);
    pinMode(tr2,OUTPUT);
 //   lcd.begin(16, 2);
 //   lcd.setCursor(0,0);
 //   lcd.print("AQUA MNTRNG");
    feed.attach(9);
    pinMode(mp1,OUTPUT);
   // pinMode(mp2,OUTPUT);
    pinMode(m1,OUTPUT);
    pinMode(m2,OUTPUT);
    pinMode(m3,OUTPUT);
    pinMode(m4,OUTPUT);
     wifi_init();
    feed.write(0);
    digitalWrite(mp1,0);
    digitalWrite(mp2,0);
    pinMode(ir,INPUT);
}

void loop() {
 int sensorValue = analogRead(sensorPin);
 Serial.println(sensorValue);
 int turbidity = map(sensorValue, 0, 750, 100, 0);
 mySerial.println("T:");
 mySerial.println(turbidity);
 int gval=analogRead(gas);
 mySerial.println("AMMONIA:");
 mySerial.println(gval);
digitalWrite(tr1,1);
delay(0.001);
digitalWrite(tr1,0);
int dst1=pulseIn(ec1,1)/58.2;
digitalWrite(tr2,1);
delay(0.001);
digitalWrite(tr2,0);
int dst2=pulseIn(ec2,1)/58.2;
mySerial.println("D1:");
mySerial.println(dst1);
mySerial.println("D2:");
mySerial.println(dst2);
delay(1000);
 int irval=digitalRead(ir);
 sensors.requestTemperatures();
 int tval = sensors.getTempCByIndex(0);
 mySerial.println("WT:");
 mySerial.println(tval);
//lcd.clear();
Serial.print("T:"+ String(tval) + " L:"+ String(dst1) + "L1:" + String(dst2));
 //lcd.setCursor(0,1);
// lcd.print("IR:" + String(irval));
    if(irval==0)
    {
      feed.write(90);
      delay(200);
    }
    else
    {
   
      feed.write(0);
     
    }
    if(dst1<20)
    {
      digitalWrite(m1,0);
      digitalWrite(m2,1);
      digitalWrite(m3,1);
      digitalWrite(m4,0);
      
    }
    else
    {
     digitalWrite(m1,1);
     digitalWrite(m2,0); 
     digitalWrite(m3,1);
     digitalWrite(m4,0);
    }
   if(dst2<20)
    {
      digitalWrite(mp1,0);
      digitalWrite(mp2,0);
    }
   else
   {
    digitalWrite(mp1,1);
    digitalWrite(mp2,0);
   }
    delay(300);
    if(kk>25)
    {
      kk=0;
    
    upload(tval,dst1,dst2,irval,turbidity,gval);
}
kk=kk+1;
}
    void wifi_init()
 {
  mySerial.println("AT+RST");
  delay(4000);
  mySerial.println("AT+CWMODE=3");
  delay(4000);
  mySerial.print("AT+CWJAP=");
  mySerial.write('"');
  mySerial.print("SRC 24G"); // ssid/user name
  mySerial.write('"');
  mySerial.write(',');
  mySerial.write('"');
  mySerial.print("src@internet"); //password
  mySerial.write('"');
  mySerial.println();
  delay(1000);
 }


void upload(int x, int y, int z,int p,int q,int r) //ldr copied int to - x   and gas copied into -y
{
  
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  mySerial.println(cmd);
  delay(1000);
  String getStr ="GET /update?api_key=VJV2DQFSGOXX3KOR&field1=";
  getStr += String(x);
  getStr +="&field2=";
  getStr += String(y);
  getStr +="&field3=";
  getStr += String(z);
  getStr +="&field4=";
  getStr += String(p);
  getStr +="&field5=";
  getStr += String(q);
  getStr +="&field6=";
  getStr += String(r);
  getStr += "\r\n\r\n";
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  mySerial.println(cmd);
  delay(1000);
  mySerial.println(getStr);
}
