
//setup
#include <SoftwareSerial.h>
#include <stdlib.h>
int ledPin = 13;// LED
int lm35Pin = A0;// LM35 analog input
String apiKey = "ZP1PZO62773HXWVU";//thingspeak API key
// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10, 11); // RX, TX



// runs once
void setup() {
pinMode(ledPin, OUTPUT);// initialize the digital pin as an output.
Serial.begin(9600);// enable debug serial
ser.begin(9600);// enable software serial
ser.println("AT+RST");// reset ESP8266
}


// loop
void loop() {

// blink LED on board
digitalWrite(ledPin, HIGH);
delay(200);
digitalWrite(ledPin, LOW);

// read the value from LM35.
// read 10 values for averaging.
int val = 0;
for(int i = 0; i < 10; i++) {
val += analogRead(lm35Pin);
delay(500);
}

// convert to temp:
// temp value is in 0-1023 range
// LM35 outputs 10mV/degree C. ie, 1 Volt => 100 degrees C
// So Temp = (avg_val/1023)*5 Volts * 100 degrees/Volt
float temp = val*50.0f/1023.0f;

// convert to string
char buf[16];
String strTemp = dtostrf(temp, 4, 1, buf);

Serial.println(strTemp);

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
getStr +="&field1=";
getStr += String(strTemp);
getStr += "\r\n\r\n";

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

// thingspeak needs 15 sec delay between updates
delay(16000);
}
