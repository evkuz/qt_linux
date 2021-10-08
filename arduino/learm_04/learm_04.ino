/*
 * learm_04.ino
 *
 * Данные передаем/принимаем в двоичном виде.
 *
 *
 * learm_03.ino
 * Двигаемся дальше. Добавляем фиксированные команды.
 * Т.е. из начального положения - взять предмет и переместить.
 * Обнаружилась проблема с парсером строки, приходящей из ПК.
 * Перехожу к следующему проекту, чтобы этот сохранился.
 *
 * learm_02.ino
 * Настриваем прием, парсинг и выполнение команд от serial порта. [сделано 24.05.2021]
 * 
 * learm_01.ino
 * Готовая база для развития проекта
 * Роботу надо сообщать углы (конечное положение)+ для приводов, далее он сам передвигает ПЛАВНО двигатели в нужную позицию
*/

#include <Arduino.h>
#include <Servo.h>
//#include "/home/evkuz/0_arduino/include/hiwonder_byte.h"
//#include "move_servos.ino"

///home/evkuz/0_arduino/include/hiwonder_byte.h
///home/evkuz/lit/learm/include/hiwonder_byte.h

// /home/evkuz/lit/learm/
/// //../include/hiwonder_byte.h
#include <stdlib.h>


#define serv_number 6 // Количество приводов под управлением
#define sBufSize 32   // Размер буфера компорта в плате NANO - 64 байта.
#define szParcel 8
Servo servo1, servo2, servo3,servo4,servo5,servo6;
Servo servos [6] = {servo1, servo2, servo3,servo4,servo5,servo6};


int *s1, *s2, *s3, *s4, *s5, *s6;
byte current_s [6]; // Текущее значение угла для соответстующего привода 0-180
byte readed_pos[6];
byte delta [6];     // Разница (между текущим и целевым положением) в угле для соответствующего привода 0 - 180
//String message, number;//, s_pos;
char *s_pos;
char buf[sBufSize];

byte ints[sBufSize]; // Данные, полученные по serial
short DF [6] ={1, 1, 1, 1, 1, 1};

//++++++++++++++++++++++++ setup
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1000);
  while (!Serial) {

      ; // wait for serial port to connect. Needed for native USB port only

    }
// attach servos to correspondent pin
  for (int i=0; i<= serv_number -1; i++)  { servos[i].attach(i+2); } //, 500, 2500;

//  move_servo_together(hwr_Start_position, 1, 6);
  delay(1000);


  for (byte i=0; i< sBufSize; i++){
      ints[i] = 93;
      buf[i] = 255;
  }

} //setup()
//++++++++++++++++++++++++ loop 
void loop() {

//int inByte;
parse_command();

/*
    if (Serial.available() > 0) {

        // get incoming byte:

        inByte = Serial.read();
        Serial.print("NANO received: ");
        Serial.println(inByte, DEC);
        Serial.flush();

    }
*/

}//loop
//+++++++++++++++++++++++++++++++++++ to_fix_position(byte *pos)
/*
void to_fix_position (*pos)
Перейти в позицию из набора фиксированных.
параметр - указатель на массив значений углов приводов.
*/
void to_fix_position(byte *pos) { for (int i=0; i<= serv_number -1; i++) { servos[i].write(pos[i]); delay(15); }
}

//++++++++++++++++++++++++++++++++++ start 
// А вообще, на будущее, надо чтобы сначала считывала текущую позицию и 
// и на основании этого вычисляла количество шагов, чтобы было плавное 
// позиционирование в начальную позицию.
//void start_pozition() { to_fix_position(hwr_Start_position); }//start_pozition
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


//++++++++++++++++++++++++++
void parse_command ()
{
    String message;

//    static byte prevAm = 0;
//    static uint32_t tmr = 0;
//    byte am = Serial.available();
//    if (am != prevAm){
//        prevAm = am;
//        tmr = millis();
//    }
//    if ((((am & millis()) - tmr) >10) || am >60){
//        uint32_t us = micros();


//    }

    if (Serial.available()) {
      //byte ints[64];           // массив для численных данных, у нас 6 приводов
      byte numReaded;
      
      numReaded=Serial.readBytes(ints, szParcel);
      //Serial.print(numReaded);
     // Serial.write(numReaded);
      //message = "Robot just got data : ";
      message = "";
      for (int i=0; i<numReaded; i++)
      {          message += String(ints[i]); message += " ";

      }

      message.remove(message.length()-1);
      strcpy(buf, message.c_str());
      Serial.print(buf);

      //Serial.println(message);
      //Serial.println("Old macDonald have a farm 12345 very very well !!!!"); //51
      //Serial.flush();

      //move_servo_together(ints, numReaded);
      Go_To_Position(ints);
      /*Now send current servo data to PC*/
     // get_all_servos();

    }//if (serial.available())

/*
    switch (data) {

    case 0x55:
        clamp();
        break;
    default:
        message = "Wrong data !!!";
        Serial.println(message);
        Serial.flush();
    }
*/
}//parse_command
//++++++++++++++++++++++
void Go_To_Position(byte *pos)
{
  String message;

 /*
  * Для позиций у дальнего края надо вводить поправку.
  * Если серво 5 больше 135, то привод 4 двигаем сначала на половину,
  * потом открываем захват, и потом уже 4 и 3 приводы до конца.

*/
    switch (pos[6]) {

    case 0x31: // Движение "Туда"

        if (pos[7]==0xE9) //0xE9==233  // Предпоследняя команда - положить кубик на тележку.
        {
            move_servo_together (ints, 6, 6);
            delay(500);
            move_servo_together (ints, 3, 4);
            delay(500);
            move_servo_together (ints, 5, 5);
            delay(1000);
            move_servo_together (ints, 1, 1);
            delay(1000);
//            move_servo_together (ints, 3, 3);
//            delay(1000);

        }

        else {
           move_servo_together (ints, 4, 6);
           delay(1000);
           move_servo_together (ints, 1, 1);
           delay(1000);
           move_servo_together (ints, 3, 3);
           delay(1000);
        }

      break;

          
    case 0x30: // Движение "Обратно"
          if (pos[7]==0xC8) 
          {// Не последняя команда, то как обычно

          move_servo_together (ints, 3, 3);
          delay(1000);
          move_servo_together (ints, 1, 1);
          delay(1000);
          move_servo_together (ints, 4, 6);
          delay(1000);
          }

           if (pos[7]==0xF4){
               move_servo_together (ints, 3, 5);
               delay(500);
               move_servo_together (ints, 1, 6);
               delay(500);

           }


          if (pos[7]==0xDE)
          {// Последняя команда, может быть и одиночой, но на случай работы с кубиком делаем так
            
          move_servo_together (ints, 3, 4);
          delay(1000);

          move_servo_together (ints, 5, 6);
          delay(500);
          move_servo_together (ints, 1, 1);

          }   
     break; //case 0x30:




    default:
        message = "Wrong data !!!";
        Serial.println(message);
        //Serial.flush();
        
    }//switch (pos[6])
    
if (pos[7]==0xDE) {
   message = "Robot movement DONE! LAST !!"; 
  }
  else {
   message = "Robot movement DONE! Total!!"; //28 bytes  //message += String(numBytes);}
  }
    
   // message = "Robot movement DONE! I like to move it move it";
//    byte mystrlen = message.length();

    //Serial.println(message);
    //Serial.write(message, 28); //sizeof(message)

    strcpy(buf, message.c_str());
    Serial.print(buf);
   // Serial.flush();
}//Go_To_Position
//+++++++++++++++++++++++++++++
