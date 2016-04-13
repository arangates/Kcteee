/* 
In this program ,When a AT command is entered in to the serial monitor of the computer 
the Arduino will send it to the ESP8266 and also reads back

 Arduino pin 2 (RX) to ESP8266 TX
 Arduino pin 3 to voltage divider then to ESP8266 RX
 Connect GND from the Arduiono to GND on the ESP8266
 ESP8266 CH_PD HIGH ---> 3.3v
*/


#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);  //RX,TX
 
int LEDPIN = 13;
 
void setup() 
{
    pinMode(LEDPIN, OUTPUT);
 
    Serial.begin(9600);     // communication with the host computer
    //while (!Serial)   { ; }
 
    // Start the software serial for communication with the ESP8266
    mySerial.begin(115200);  
 
    Serial.println("");
    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
    Serial.println("Ready");
    Serial.println("");    
}
 
void loop() 
{
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( mySerial.available() )   {  Serial.write( mySerial.read() );  }
 
    // listen for user input and send it to the ESP8266
    if ( Serial.available() )       {  mySerial.write( Serial.read() );  }
}
