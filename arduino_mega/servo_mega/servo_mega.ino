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
#include <stdlib.h>

#define serv_number 4 // Количество приводов под управлением
#define sBufSize 64   // Размер буфера компорта в плате NANO - 64 байта.

Servo servo1, servo2, servo3,servo4;
Servo servos [serv_number] = {servo1, servo2, servo3,servo4};

byte current_s [serv_number]; // Текущее значение угла для соответстующего привода 0-180
byte ints[sBufSize]; // Данные, полученные по serial
byte delta [serv_number];     // Разница (между текущим и целевым положением) в угле для соответствующего привода 0 - 180
short DF [serv_number] ={1, 1, 1, 1};

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.setTimeout(1000);
  while (!Serial) {

      ; // wait for serial port to connect. Needed for native USB port only

    }
  
// attach servos to correspondent pin
  for (int i=0; i<= serv_number -1; i++)  { servos[i].attach(i+2); } //, 500, 2500;

  for (byte i=0; i< sBufSize; i++){
      ints[i] = 93;
//      buf[i] = 255;
  }


}

// the loop function runs over and over again forever
void loop() {
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(250);                       // wait for a second
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  delay(250);    
  ints[0] = 30;
  move_servo_together (ints, 1, 1);
  delay(750);
   ints[0] = 100;
  move_servo_together (ints, 1, 1);
  delay(750);

  // wait for a second
//  servo1.write(45);
//  delay(50);
//  servo2.write(0);
//  
//  delay(1500);
//  servo1.write(90);
//  delay(50);
//  servo2.write(180);
//  delay(1500);

  
}
//+++++++++++++++++++++++++++++++++++
/*
Получить значения углов для всех приводов
параметр when означает - ДО начала движения и В КОНЦЕ движения
*/
void get_all_servos(String when)
{
    String message;
    message = "From robot "; message += when; message += " get_all_servo  :  ";
    for (int i=0; i<=serv_number - 1; i++)
    {

      current_s[i] = servos[i].read(); //Current servo
      //readed_pos[i] = current_s[i];
        //  message += String(i); message += " position ";
      message += String(current_s[i]);  message += ", ";
    //
    }
  //  Serial.println(message);
  //  Serial.flush();
}//get_all_servos()

//+++++++++++++++++++++++++++++++++++
/*
  Задаем значения приращения угла и направление для всех приводов для текущей и целевой позиции
*/
void get_curr_delta (byte *pos)
{

  for (int i=0; i<serv_number; i++)
  {
    if (current_s[i] > pos[i])
      {
       delta[i] = current_s[i] - pos[i];
       DF[i] = -1;
      }
    else
      {
        delta[i] = pos[i] - current_s[i];
       DF[i] = 1;
      }

  }//for


}//get_curr_delta
