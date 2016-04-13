ESP8266 Wifi Module is an integrated chip designed for communicating to the world of the internet through Wifi radio signals. It is the link between a project and the internet. It has on-board processing and memory that allows it to be integrated with electronics through its GPIOs. The ESP8266 can be used to connect your Arduino projects to the internet.

In this instructable, you will get started with the basics of ESP8266 and how to program it using an AT commands.

Step 1: Parts

These parts are needed to program your ESP8266:

1 - ESP8266

2 - Arduino Uno

3 - Resistor 330 Ω

4 - Resistor 180 Ω

5 - Jumper Wires

6 - Mini Breadboard

Step 2: Connecting the ESP8266

One of the ways to programming the ESP8266 is throughout the Arduino micro-controller board. The ESP8266 is programmed using AT commands; when received, it replies with an acknowledgment. The ESP8266 is powered up using a 3.3 Volts power source, An Arduino Uno will be able to power up the ESP through its regulated 3.3 V power pin.

The ESP8266 is connected to the Arduino through TXD, RXD, GND, and the VCC pins to the RX, TX, GND and 3.3 V pins, respectively (If you are using an Arduino MEGA). The TX received from the Arduino should be connected to a voltage divider so that RXD of the ESP8266 receives a signal of 3.3 volts logic and not 5 volts. If the signal is received in 5 volts, the ESP8266 will malfunction. The voltage divider will be explained in the next step.

If you are using an Arduino Uno, then follow the steps:

Arduino --------------------------------------------------- ESP8266

Pin 10 (Software RX) -----------------------------------> TXD

Pin 11 (Software TX) -------> Voltage Divider -----> RXD

GND --------------------------------------------------------> GND

3.3 V --------------------------------------------------------> VCC

3.3 V --------------------------------------------------------> CH_PD

Step 3: The voltage divider

 Picture of The voltage divider 
The voltage divider is used to produce an output voltage that is a fraction of the input voltage. We will use the voltage divider between the TX of the Arduino and RXD of the ESP8266 to produce an output voltage of 3.3 V.

I have used a 180 Ω resistor and a 330 Ω resister, as R1 and R2, respectively.

Use this website if you have different resistor values.

Step 4: Programming

After the connection of the pins mentioned above, the following code is uploaded to the Arduino to enable serial communication between the serial monitor and the ESP8266 through the Arduino. When a command is entered into the serial monitor on the computer the Arduino will relay it to the ESP8266. Download the Arduino code attached and upload it to the Arduino Uno board.

 WriteToESP_UNO.ino WriteToESP_UNO.ino
Step 5: AT commands

 Picture of AT commands
There are many AT commands that can be used to program the ESP8266 Wifi module. Check them out here. The following AT commands should be written in the Arduino’s serial monitor (in order) to program the ESP8266:

“AT” This will check if the module is connected properly and its functioning, the module will reply with an acknowledgment.
“AT+RST” This will reset the wifi module. Its good practice to reset it before or after it has been programmed.
“AT+GMR” This will mention the firmware version installed on the ESP8266. (Optional)
“AT+CWLAP” This will detect the Access points and their signal strengths available in the area.
AT+CWJAP=”SSID”,”PASSWORD” This connects the ESP8266 to the specified SSID in the AT command mentioned in the previous code.
“AT+CIFSR” This will display the ESP8266’s obtained IP address.
If the user wants to disconnect from any access point then use the following AT command AT+CWJAP=””,””
“AT+CWMODE=1” This sets the Wifi mode. It should be always set to Mode 1 if the module is going to be used as a node (Like our mobile’s connection to the access points)
After this step is done, repeat step 2 to reset the Wifi Module. “AT+RST”
Now you can connect your ESP8266 to the internet and get started with IoT.