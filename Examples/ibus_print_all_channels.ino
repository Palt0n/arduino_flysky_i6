#include <IBusBM.h>
#include <Servo.h>


/*
  Translate iBUS signal to servo
  
  Supports any Arduino board where serial0 is available. 
 
  For boards where serial0 is connected to the onboard USB port (such as MEGA, UNO and micro) you need
  to disconnect the RX line from the receiver during programming. 

  Alternatively you can change the code below to use another serial port.

  Please use 5V boards only.

  Serial port RX/TX connected as follows:
  - RX connected to the iBUS servo pin (disconnect during programming on MEGA, UNO and micro boards!)
  - TX left open or for Arduino boards without an onboard USB controler - connect to an 
    USB/Serial converter to display debug information on your PC (set baud rate to 115200).  

*/

IBusBM IBus; // IBus object
Servo myservo;  // create servo object to control a servo

void setup() {
  // Serial.begin(115200);   // remove comment from this line if you change the Serial port in the next line

  IBus.begin(Serial);    // iBUS connected to Serial0 - change to Serial1 or Serial2 port when required

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  Serial.println("Start IBus2PWM");
}

int array_val[10] = {0}; // all elements 0

int transform_ch2_to_y(int value) {
  if(value > 0){
    float result = (value - 1500.0) * 255 / 500;
    /* Transform 1000us - 2000us values from flysky RX to 0 - 255 */
    return round(result);
  }
  else {
    return 0;
  }
}

int transform_ch1_to_x(int value) {
  return transform_ch2_to_y(value);
}

void loop() {
  int val;
  for (int channel = 0; channel < 10; channel++) {
    val = IBus.readChannel(channel); // get latest value for servo channel 1
    if (array_val[channel] != val) {
      myservo.writeMicroseconds(val);   // sets the servo position 
      array_val[channel] = val;
    }
    Serial.print("CH");
    Serial.print(channel);
    Serial.print(":");
    Serial.print(val);
    if (channel == 0) {
      Serial.print("=");
      Serial.print(transform_ch1_to_x(val));
    }
    if (channel == 1) {
      Serial.print("=");
      Serial.print(transform_ch2_to_y(val));
    }
    Serial.print(" ");
  }
  Serial.println("");
  delay(100);
}

