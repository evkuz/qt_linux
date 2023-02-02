#include <Arduino.h>

void Am1()
{
  cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
//  if (micros() - lastTurn < pause) return;  // Если с момента последнего изменения состояния не прошло
//  // достаточно времени - выходим из прерывания
  pinAm1Value = digitalRead(pinAm1);            // Получаем состояние пинов A и B
//  pinBm1Value = digitalRead(pinBm1);

intM1counter++;
  if (pinAm1Value >0) {
    posAm1++;
    }
  else {
    posAm1--;
    }
sei(); // Разрешаем обработку прерываний

}
//+++++++++++++++++++++++++++++++++++
void Bm1()
{
    cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
//  if (micros() - lastTurn < pause) return;
//  pinAm1Value = digitalRead(pinAm1);
  pinBm1Value = digitalRead(pinBm1);


  if (pinBm1Value >0) {
    posBm1++;
    }
  else {
    posBm1--;
    }
sei(); // Разрешаем обработку прерываний

}
//++++++++++++++++++++++++++++++++++++++++++
void setCount(int state) {          // Устанавливаем значение счетчика
  if (state == 4 || state == -4) {  // Если переменная state приняла заданное значение приращения
    count += (int)(state / 4);      // Увеличиваем/уменьшаем счетчик
    lastTurn = micros();            // Запоминаем последнее изменение
  }
}
//++++++++++++++++++++++++++++++++++++++++++
void Am2()
{
    cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
//  if (micros() - lastTurn < pause) return;  // Если с момента последнего изменения состояния не прошло
//  // достаточно времени - выходим из прерывания
intM2counter++;
  pinAm2Value = digitalRead(pinAm2);            // Получаем состояние пинов A и B
//  pinBm2Value = digitalRead(pinBm2);

  if (pinAm2Value >0) {
    posAm2++;
    }
  else {
    posAm2--;
    }
sei(); // Разрешаем обработку прерываний
} // Am2()
//+++++++++++++++++++++++++++++++
void Bm2()
{
    cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
    intM2Bcounter++;
//  if (micros() - lastTurn < pause) return;
//  pinAm2Value = digitalRead(pinAm2);
  pinBm2Value = digitalRead(pinBm2);

  if (pinBm2Value >0) {
    posBm2++;
    }
  else {
    posBm2--;
    }
  sei(); // Разрешаем обработку прерываний
} // Bm2
//+++++++++++++++++++++++++++++++
void reset_All()
{
  posAm1 = 0;
  posBm1 = 0;
  posAm2 = 0;
  posBm2 = 0;
  m1_count = 0.0;
  m2_count = 0.0;
  m1Speed = defaultMSpeed;
  m2Speed = defaultMSpeed;

  }
