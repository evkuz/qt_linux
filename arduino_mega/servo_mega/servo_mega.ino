/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
#include <Servo.h>

#define serv_number 6 // Количество приводов под управлением
#define sBufSize 64   // Размер буфера компорта в плате NANO - 64 байта.

Servo servo1, servo2, servo3,servo4,servo5,servo6;
Servo servos [6] = {servo1, servo2, servo3,servo4,servo5,servo6};


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.begin(115200);
  
  servo1.attach(3);
  servo2.attach(5);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(250);                       // wait for a second
  servo1.write(150);
 // delay(50);
//  servo2.write(150);
  
  delay(1500);
  servo1.write(20);
  //delay(50);
 // servo2.write(20);
  delay(1500);

  
}
