//#include "move_servos.h"

// void move_servo_together (byte *pos, byte start_servo, byte end_servo)
// /*Возвращает индекс макисмального элемента в массиве delta*/
// byte get_max_delta (byte *arr, byte start_servo, byte end_servo)
// Attach all servos. need for smooth start
// void smoothStart()

void move_servo_together (byte *pos, byte start_servo, byte end_servo) // address of position array and direction flag array, текущую позицию вычисляем
{
  byte s_pos, maxdt;
  String message;
  get_all_servos("before"); // Получаем массив current_s[] - текущие значения углов сервоприводов
  get_curr_delta(pos);      // Получаем массив delta и направление (массив - DF)
  maxdt = get_max_delta(delta, start_servo, end_servo); // индекс в массиве delta, а не абсолютное значение/
/*  message = "Servo index with max delta is ";
  message += String(maxdt);
  Serial.println(message);
  Serial.flush();

  message = "Max delta value is ";
  message += String(delta[maxdt]);

  Serial.println(message);
  Serial.flush();
  message = "Delta values are : ";
  for (byte i=0; i<= serv_number -1; i++){ message += String(DF[i]); message += ", ";}
  Serial.println(message);
  Serial.flush();
*/


  /*   message = "Current Max delta value is ";
message += String(delta[maxdt]);
Serial.println(message);
Serial.flush();
 */


  byte cycle_num =0;
while (maxdt != 100) // Перебираем дельты с наибольшим значением пока таковое не станет нулевым.
  {                    // см. get_max_delta()

     //
    for (byte i=start_servo -1; i< end_servo ; i++) //
    {                                                //

        s_pos = servos[i].read();
        if (delta[i] !=0)
        {
            s_pos = s_pos + DF[i];
            servos[i].write(s_pos);
            delta[i] -= 1;
            delay(10);
        }

    }//for (byte i=start_servo -1; // вся текущая макс. дельта
    // Обошли все приводы. Определяем новое значение maxdt
    /*
        И вот тут засада, т.к. максимальное дельта может быть у привода не из списка start_servo - end_servo
*/
    maxdt = get_max_delta(delta, start_servo, end_servo);
    cycle_num++;

}//while (maxdt != 100)

// Посылаем текущие позиции после завершения движений.
get_all_servos("after");
/*
И вот тут надо бы сравнить, что пришло и что сейчас.
*/
// Сравниваем массивы
// message += "Main Cycle worked "; message += String(cycle_num); message += " times";

//for (byte i=0; i<=serv_number -1; i++){ // ОБходим все приводы, проверяем совпадение позиции с заданной
//  if (current_s[i] != pos[i]){
//      message = "Позиция не совпадает с заданной, привод ";
//      message += String(i); message += "\n";
//      message += "Задано      "; message += String(pos[i]);
//      message += "Фактическая "; message += String(current_s[i]);
//      Serial.println(message);

//  }

//}



//message = "Robot movement DONE!"; //message += String(numBytes);
//byte mystrlen = message.length();
//while ( message.length() <=61){
//    message += " ";//String(9);
//    //byte a = 120;
//}
//Serial.println(message);
//Serial.flush();

}//move_servo_together

//+++++++++++++++++++++++++++++++++++++++++++++++++
/*Возвращает индекс макисмального элемента в массиве delta*/
byte get_max_delta (byte *arr, byte start_servo, byte end_servo)
{
  int maxdelta, index;
  maxdelta=0;
  index=0;
  for (int i=start_servo-1; i<end_servo; i++)
  {
    if (arr[i] > maxdelta)
    {
      maxdelta = arr[i];
      index = i;
    }

  }//for

  if (maxdelta==0)
  { // Перебрали все дельты и все теперь равны 0
    index = 100; // Признак завершения движения приводов.
  }

  return index;
} //get_max_delta
//++++++++++++++++++++++++++
// Attach all servos. need for smooth start
void smoothStart()

{
    for (byte i=0; i< serv_number; i++) //
    {
      for (byte j=0; j< 10; j++) {
        servos[i].detach();
        delay(75);
        servos[i].attach(i+2);
        // writeUs(_servoCurrentPos);
        delay(25);
      }

    }

}
//++++++++++++++++++++++++++++
// В мобильном манипуляторе, при захвате лучше двигать приводы одновремено.
void move_servos(byte *pos)
{
    byte s_pos, maxdt, counter;
      String message;
      get_all_servos("before"); //Получаем массив current_s[]
      get_curr_delta(pos);
      maxdt = get_max_delta_00(delta); // индекс в массиве delta, а не абсолютное значение/
    /*  message = "Servo index with max delta is ";
      message += String(maxdt);
      Serial.println(message);
      Serial.flush();

      message = "Max delta value is ";
      message += String(delta[maxdt]);

      Serial.println(message);
      Serial.flush();
      message = "Delta values are : ";
      for (byte i=0; i<= serv_number -1; i++){ message += String(DF[i]); message += ", ";}
      Serial.println(message);
      Serial.flush();
    */
    while (maxdt != 100) // Перебираем дельты с наибольшим значением пока таковое не станет нулевым.
    {                    // см. get_max_delta()
        counter = delta[maxdt];
      for (byte dt=0; dt < counter; dt++) //Берем дельту по индексу из массива
       {
    /*     message = "Current Max delta value is ";
         message += String(delta[maxdt]);
         Serial.println(message);
         Serial.flush();
    */
         for (byte i=0; i< serv_number - 1; i++) // ОБходим все приводы
           {
            s_pos = servos[i].read();
        if (delta[i] !=0)
        {
          s_pos = s_pos + DF[i];
          servos[i].write(s_pos);
          delta[i] -= 1;
          delay(10);
        }
        }//for int i=0 // все приводы

      }//for int dt=0; // вся текущая макс. дельта
        // Обошли все приводы. Определяем новое значение maxdt
        maxdt = get_max_delta_00(delta);

    }//while (maxdt != 100)

    // Посылаем текущие позиции после завершения движений.
    get_all_servos("after");
    /*
    И вот тут надо бы сравнить, что пришло и что сейчас.
    */

}
//++++++++++++++++++++++++++++++
/*Возвращает индекс макисмального элемента в массиве delta*/
byte get_max_delta_00 (byte *arr)
{
  int maxdelta, index;
  maxdelta=0;
  index=0;
  for (int i=0; i<serv_number -1; i++)
  {
    if (arr[i] > maxdelta)
    {
      maxdelta = arr[i];
      index = i;
    }

  }//for

  if (maxdelta==0)
  { // Перебрали все дельты и все теперь равны 0
    index = 100;
  }

  return index;
} //get_max_delta
