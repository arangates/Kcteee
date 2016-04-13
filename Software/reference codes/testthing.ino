#include <SoftwareSerial.h>

#define DEBUG

// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000) 
#define BCOEFFICIENT 3950
// the value of the 'other' resistor measured with my volt meter
#define SERIESRESISTOR 9870    
//*-- Software Serial
#define _rxpin      10
#define _txpin      11
SoftwareSerial debug( _rxpin, _txpin ); // RX, TX
//*-- IoT Information
#define SSID "networkname"
#define PASS "password"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149
//*-- Build GET Command
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&filed2=[data 2]...;
String GET = "GET /update?key=APIKEY";

int samples[NUMSAMPLES];
 
void setup(void) {
  Serial.begin(9600);
  analogReference(DEFAULT);
  Serial.println("AT+RST");
  debug.println("ESP8266");
  delay(1000);
  if(Serial.find("ready"))
  {
    debug.println("ESP8266 CONNECTED\nREADY TO SEND DATA");  
    connectWiFi();
  }
}
 
void loop(void) {
  delay(30000);
  // ESP8266 Client
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  debug.println(cmd);
  delay(2000);
  if(Serial.find("Error"))
    {
      debug.print( "CANNOT CONNECT\nEXITING" );  
      return;
    }
  uint8_t i;
  float average;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  //Serial.print("Average analog reading "); 
  //Serial.println(average);
 
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  //Serial.print("Thermistor resistance "); 
  //Serial.println(average);
 
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  steinhart = steinhart * 1.8 + 32;            // convert to F
  //Serial.print("Temperature "); 
  //Serial.print(steinhart * 1.8 + 32);
  //Serial.println(" *F");

  cmd = GET + "&field1=" + steinhart + "\r";
  Serial.print("AT+CIPSEND=");
  debug.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  debug.println(cmd.length());
  
  if(Serial.find(">"))
  {
    debug.print(">");
    Serial.print(cmd);
    debug.print(cmd);
  }
  else
  {
    Serial.println( "AT+CIPCLOSE" );
    debug.println( "AT+CIPCLOSE" );
  }
  if( Serial.find("OK") )
  { 
    debug.println( "DATA SENT" );
  }
  else
  {
    debug.println( "DATA NOT SENT\nEXITING" );    
  }
  delay(1000);
}

boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");
  debug.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK"))
  {
      debug.println("WIFI CONNECTED");
      delay(1000);
      Serial.println("AT+CIFSR");
      while (Serial.available())
        debug.write(Serial.read());
  }
  else
  {
      debug.println("ERROR CONNECTING TO WIFI");
      return false;
  }
  delay(2000);
  cmd = "AT+CIPMUX=0";
  Serial.println( cmd );
  debug.println( cmd );
    
  if( Serial.find( "Error") )
  {
      debug.print( "CANNOT SET SINGLE CONNECTION MODE" );
      return false;
  }
}
