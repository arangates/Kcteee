#include <SoftwareSerial.h>
#include <stdlib.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

int sensorPin0 = A0;// R -voltage
int sensorPin1 = A1;// Y -voltage
int sensorPin2 = A2;// B -voltage

int sensorPin3 = A3;// R -current
int sensorPin4 = A4;// Y -current
int sensorPin5 = A5;// B -current

int speed; //speed
int temperature; //temperature

const int ledPin = 5; // speedsensor?
const int rlyPin = 6;
int sensorState = 0; // tempsensor ?

String apiKey = "ETWI0YZCDOHZA6BN"; //Thingspeak api

// connect 2 to TX of Serial USB [GREEN]
// connect 4 to RX of serial USB [ORANGE]
SoftwareSerial ser(3,4); // RX, TX any digital pin

int k=0,motoff;
int r_volt,y_volt,b_volt,sec,msec;
int r_amp,y_amp,b_amp,interruptt,sped;

void decimal3(unsigned int col,unsigned int row,unsigned int value);
void decimal3c(unsigned int col,unsigned int row,unsigned int value);
void Lcd8_Decimal4(unsigned int col,unsigned int row,unsigned int val);

//unsigned int current_measure(char channel);
unsigned int calibrate(unsigned int adc_val);
void timer_init();

void setup()
 {

    pinMode(ledPin, INPUT);
    pinMode(rlyPin, OUTPUT);

    //esp setup
    Serial.begin(9600);// enable debug serial
    ser.begin(115200);// enable software serial
    ser.println("AT+RST");// restart ESP8266

    attachInterrupt(0, blink1, RISING);
     digitalWrite(rlyPin, LOW);
     // Serial.begin(9600);
      lcd.begin(20, 4);lcd.setCursor(0, 1);
      lcd.print("MONITORING&CNTRLLING");
      lcd.setCursor(0, 2);
      lcd.print("OF 3 PHASE INDUCTION");
      lcd.setCursor(0, 3);
      lcd.print("         MOTOR      ");
      delay(3000);delay(3000);
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      //delay(3000);
      lcd.setCursor(0, 1);
      lcd.print("V");
      lcd.setCursor(3, 0);
      lcd.print("R:");
      lcd.setCursor(7, 0);
      lcd.print("Y:");
      lcd.setCursor(11, 0);
      lcd.print("B:");
      lcd.setCursor(0, 2);
      lcd.print("A");
      lcd.setCursor(0, 3);
      lcd.print("S:"); 
      timer_init();
}
void loop()
{
r_volt = analogRead(sensorPin0);
y_volt = analogRead(sensorPin1);
b_volt = analogRead(sensorPin2);

// speed and temperature
speed = random(1100,1200);
temperature = random(30,35);

//decimal3(3,1,msec);
/*decimal3(3,3,msec);
decimal3(7,3,interruptt);*/
Lcd8_Decimal4(3,3,sped);
r_volt=r_volt>>2;
decimal3(3,1,r_volt);
y_volt=y_volt>>2;
decimal3(8,1,y_volt);
b_volt=b_volt>>2;
decimal3(12,1   ,b_volt);
r_amp=1.732*analogRead(sensorPin3);
y_amp=1.732*analogRead(sensorPin4);
b_amp=1.732*analogRead(sensorPin5);
r_amp=r_amp>>2;
//decimal3c(3,3,r_amp);
y_amp=y_amp>>2;
//decimal3c(8,3,y_amp);
b_amp=b_amp>>2;
//decimal3c(13,3,b_amp);
/*r_amp=calibrate(r_amp);
y_amp=calibrate(y_amp);
b_amp=calibrate(b_amp);*/
decimal3c(3,2,r_amp);
decimal3c(8,2,y_amp);
decimal3c(13,2,b_amp);
if (r_volt<180 || r_volt>240){motoff=0;}
else if (y_volt<180 || y_volt>240){motoff=0;}
else if (b_volt<180 || b_volt>240){motoff=0;}
else if (r_amp>70){motoff=0;}
else if (y_amp>70){motoff=0;}
else if (b_amp>70){motoff=0;}
else {motoff=1;}
sensorState = digitalRead(ledPin);

   // relay section
if(sensorState==1)
    {
        lcd.setCursor(7, 3);
        lcd.print("OVER TEMP  ");
    }
else
    {
        lcd.setCursor(7, 3);
        lcd.print("NORMAL TEMP");
    }
if(sensorState==0 && motoff==1)
  {
       digitalWrite(rlyPin, LOW);
  }
else
  {
       digitalWrite(rlyPin, HIGH);
  }

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
    getStr += String(r_volt);

    // field2 - Y volt
    getStr +="&field2=";
    getStr += String(y_volt);

    // field3 - B volt
    getStr +="&field3=";
    getStr += String(b_volt);

    // field4 - R Amp
    getStr +="&field4=";
    getStr += String(r_amp);

    // field5 - Y Amp
    getStr +="&field5=";
    getStr += String(y_amp);

    // field6 - B Amp
    getStr +="&field6=";
    getStr += String(b_amp);

     // field7 - speed 
    getStr +="&field7=";
    getStr += String(speed);

     // field8 - temperature
    getStr +="&field8=";
    getStr += String(temperature);

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

delay(1000); // delay for function execution  
}

unsigned int calibrate(unsigned int adc_val)
{
  int temp=0; 
  temp=((0.7407407407407407*adc_val) -71.11111111111111);
 // else temp=0;
 return temp;
}
ISR(TIMER1_COMPA_vect)
{
 // cli();
  msec++;
  //if(msec>2){msec=0;sec++;}
  //TCCR1A = 0xff;     // set entire TCCR1A register to 0
  //TCCR1B = 0x00;     // same for TCCR1B
 // sei();
 if(msec>2)
 {msec=0;sped=interruptt;interruptt=0;}
}
void timer_init()
{
  cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B 
    // set compare match register to desired timer count:
    OCR1A=20000;
    //OCR1A = 1;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    sei(); 
}
/*
unsigned int current_measure(char channel)
{
  unsigned int k,last_val,temp_val;
  for(k=0;k<=50;k++)
    {
      if(channel==3)temp_val=analogRead(sensorPin3);
      if(channel==4)temp_val=analogRead(sensorPin4);
      if(channel==5)temp_val=analogRead(sensorPin5);
      if(temp_val>=last_val)
          {
              last_val=temp_val;
          }   
    }
    return(last_val;)
}
*/
void decimal3(unsigned int col,unsigned int row,unsigned int value)
{
   lcd.setCursor(col+0, row);
   lcd.print(value/100);
   lcd.setCursor(col+1, row);
   lcd.print((value%100)/10);
   lcd.setCursor(col+2, row);
   value=(value%100);
   lcd.print(value%10);
}
void decimal3c(unsigned int col,unsigned int row,unsigned int value)
{
   lcd.setCursor(col+0, row);
   lcd.print(value/100);
   lcd.setCursor(col+1, row);
   lcd.print((value%100)/10);
   lcd.setCursor(col+2, row);
   lcd.print('.');
   lcd.setCursor(col+3, row);
   value=(value%100);
   lcd.print(value%10);
}
void Lcd8_Decimal4(unsigned int col,unsigned int row,unsigned int val) 
{
  unsigned int Lcd_th,Lcd_thr,Lcd_h,Lcd_hr,Lcd_t,Lcd_o;

  val = val%10000;
  Lcd_th=val/1000;
  Lcd_thr=val%1000;
  Lcd_h=Lcd_thr/100;
  Lcd_hr=Lcd_thr%100;
  Lcd_t=Lcd_hr/10;
  Lcd_o=Lcd_hr%10;

        lcd.setCursor(col+0, row);
        lcd.print(Lcd_th);
        lcd.setCursor(col+1, row);
        lcd.print(Lcd_h);
        lcd.setCursor(col+2, row);
        lcd.print(Lcd_t);
        lcd.setCursor(col+3, row);
        lcd.print(Lcd_o);
}
void blink1()         //External Interrupt service routine
        {             
             interruptt++;
        }
