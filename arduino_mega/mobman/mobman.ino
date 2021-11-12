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
Servo servos [serv_number] = {servo1, servo2, servo3,servo4,servo5,servo6}; // Текущие значения углов


//int *s1, *s2, *s3, *s4, *s5, *s6;
byte current_s [serv_number]; // Текущее значение угла для соответстующего привода 0-180
byte readed_pos[serv_number];
byte delta [serv_number];     // Разница (между текущим и целевым положением) в угле для соответствующего привода 0 - 180
                    // индекс элемента соответствует номеру привода, с поправкой, что индексы начинаются с 0
//String message, number;//, s_pos;
char *s_pos;
char buf[sBufSize];

byte ints[sBufSize]; // Данные, полученные по serial, заданные позиции сервоприводов
short DF [serv_number] ={1, 1, 1, 1, 1, 1}; // Значения приращений угла в градусах для приводов, т.е. 1 - увеличиваем на 1 градус,
                                  // а (-1) - уменьшаем на 1 градус.
                                  // При значении 1 - движение робота максимально плавное
                                  // Направление изменений - увеличение/уменьшение

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
Получить текущие значения углов для всех приводов
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
  Задаем значения приращения угла (массив delta) и направление (массив - DF) для всех приводов для текущей и целевой позиции
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


    if (Serial.available()) {
      //byte ints[64];           // массив для численных данных, у нас 6 приводов
      byte numReaded;
      
      numReaded=Serial.readBytes(ints, szParcel); // Считали данные из Serial в массив ints
      message = ""; 
      for (int i=0; i<numReaded; i++)
      {
          message += String(ints[i]); message += " ";
      }

      message.remove(message.length()-1);
      strcpy(buf, message.c_str());
      Serial.print(buf);

      Go_To_Position(ints);

      /*Now send current servo data to PC*/
     // get_all_servos();

    }//if (serial.available())


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
// Особый и различный порядок движения приводов в разных ситуациях
        if (pos[7]==0xE9) //0xE9==233  // Предпоследняя команда - положить кубик на тележку. Тут особый порядок.
        {
            move_servo_together (ints, 4, 4);
            delay(500);
            move_servo_together (ints, 2, 3);
            delay(500);
            move_servo_together (ints, 1, 1); // Открываем или закрываем хват
            delay(1000);
        }

        else {  // Обычный порядок - двжиение за кубиком при создании обучающей выборки
           move_servo_together (ints, 4, 4);
           delay(1000);
           move_servo_together (ints, 1, 2);
           delay(1000);
           move_servo_together (ints, 3, 3);
           delay(1000);
        }

      break;

          
    case 0x30: // Движение "Обратно"
          // Смотрим крайний, 8-й байт
          if (pos[7]==0xC8) // обычная команда
          {// Не последняя команда

          move_servo_together (ints, 3, 3); // поднимаем дальнюю половину
          delay(1000);
          move_servo_together (ints, 1, 1); // закрываем/хват
          delay(1000);
          move_servo_together (ints, 4, 4);
          delay(1000);
          }

           if (pos[7]==0xF4){ // Кубик на тележку положили, теперь грамотно убираем манипулятор (не задевая транспортир).
               move_servo_together (ints, 3, 5);
               delay(500);
               move_servo_together (ints, 1, 6);
               delay(500);
           }


          if (pos[7]==0xDE) // Последняя команда роботу при комплексном движении
          {// Последняя команда, может быть и одиночой, но на случай работы с кубиком делаем так

          move_servo_together (ints, 3, 4);
          delay(1000);
//          move_servo_together (ints, 1, 2);
//          delay(1000);

          move_servo_together (ints, 1, 2);
//          delay(300);
//          move_servo_together (ints, 6, 6);

          }   
     break; //case 0x30:




    default:
        message = "Wrong data !!!";
        Serial.println(message);
        
    }//switch (pos[6])
    
if (pos[7]==0xDE) {
   message = "Robot movement DONE! LAST !!"; 
  }
  else {
   message = "Robot current movement DONE!"; //28 bytes  //message += String(numBytes);}
  }
    

    strcpy(buf, message.c_str());
    Serial.print(buf);
   // Serial.flush();
}//Go_To_Position
//+++++++++++++++++++++++++++++
