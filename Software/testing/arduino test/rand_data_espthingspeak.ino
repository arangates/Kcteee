/*
This program sends random data to thingspeak using esp8266-01

NOTES :

esp8266 baud rate is 115200
esp tx -----> softserial RX
esp rx -----> sofserial TX
ALT+RST before starting 
all AT commands must follow \r\n

 */

#include <SoftwareSerial.h>
#include <stdlib.h>
long voltage;
long current;
long speed;
long temperature;
String apiKey = "ETWI0YZCxxxxxxx"; // Your Thingspeak api
SoftwareSerial ser(3,4); // RX, TX any digital pin of arduino

void setup(){
   //esp setup
    Serial.begin(9600);// enable debug serial for arduino
    ser.begin(115200);// enable software serial for esp
    ser.println("AT+RST");// restart ESP8266
}

void loop() {
  //Voltage
  v1 = random(240,250);
  v2 = random(240,250);
  v3 = random(240,250);

  // current
  c1= random(4,7);
  c2= random(4,7);
  c3= random(4,7);

  // speed and temperature
  speed = random(1100,1200);
  temperature = random(30,35);
  
  Serial.print("Voltage 1:");       // VOLTAGE
  Serial.print("\t");  
  Serial.println(v1);  

  Serial.print("Voltage 2:");       // VOLTAGE
  Serial.print("\t");  
  Serial.println(v2);  

  Serial.print("Voltage 3:");       // VOLTAGE
  Serial.print("\t");  
  Serial.println(v3);  
  
  
  Serial.print("current 1:");       // current
  Serial.print("\t");  
  Serial.println(c1);  
  
  Serial.print("current 2:");       // current
  Serial.print("\t");  
  Serial.println(c2);  

  Serial.print("current 3:");       // current
  Serial.print("\t");  
  Serial.println(c3);  

  Serial.print("speed:");       // speed
  Serial.print("\t");  
  Serial.println(speed);  
  
  Serial.print("temperature:");       // temperature
  Serial.print("\t");  
  Serial.println(temperature);  

// THINGSPEAK SECTION

    // TCP connection
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += "184.106.153.149"; // api.thingspeak.com
    cmd += "\",80";
    ser.println(cmd);

    if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
    }


    // prepare GET string
    String getStr = "GET /update?api_key=";
    getStr += apiKey;

    // field1 - R volt
    getStr +="&field1=";
    getStr += String(v1);

    // field2 - Y volt
    getStr +="&field2=";
    getStr += String(v2);

    // field3 - B volt
    getStr +="&field3=";
    getStr += String(v3);

    // field4 - R Amp
    getStr +="&field4=";
    getStr += String(c1);
    getStr += "\r\n\r\n";

     // field5 - Y Amp
    getStr +="&field5=";
    getStr += String(c2);

    // field6 - B Amp
    getStr +="&field6=";
    getStr += String(c3);

     // field7 - speed 
    getStr +="&field7=";
    getStr += String(speed);

     // field6 - temperature
    getStr +="&field8=";
    getStr += String(temperature);


    // send data length
    cmd = "AT+CIPSEND=";
    cmd += String(getStr.length());
    ser.println(cmd);

    if(ser.find(">")){
    ser.print(getStr);
    }
    else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
    }

  delay(1000); // but only sends once in 15 seconds 
}

