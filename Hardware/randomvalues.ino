long voltage;
long current;
long speed;
long temperature;

void setup(){
  Serial.begin(9600);

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}

void loop() {
  // print a random number from 0 to 299
  voltage = random(240,250);
  current = random(6,10);
  speed = random(1100,1250);
  temperature = analogRead(A0);
  
  Serial.print("Voltage:");       // VOLTAGE
  Serial.print("\t");  
  Serial.println(voltage);  
  
  Serial.print("current:");       // current
  Serial.print("\t");  
  Serial.println(current);  
  
  Serial.print("speed:");       // speed
  Serial.print("\t");  
  Serial.println(speed);  
  
  Serial.print("temperature:");       // temperature
  Serial.print("\t");  
  Serial.println(temperature);  

  delay(960);
}

