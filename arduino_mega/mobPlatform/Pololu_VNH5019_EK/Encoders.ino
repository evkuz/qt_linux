#include <Arduino.h>

void Am1()
{
  cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
//  if (micros() - lastTurn < pause) return;  // Если с момента последнего изменения состояния не прошло
//  // достаточно времени - выходим из прерывания
  pinAm1Value = digitalRead(pinAm1);            // Получаем состояние пинов A и B
  pinBm1Value = digitalRead(pinBm1);

intM1counter++;
  if (pinAm1Value >0) {
    posAm1++;
    }
  else {
    posAm1--;
    }
sei(); // Разрешаем обработку прерываний

//  if (pinBm1Value >0) {
//    posBm1++;
//    }
//  else {
//    posBm1--;
//    }
    
//
//  cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
//  if (state == 0  && !pinAm1Value &&  pinBm1Value || state == 2  && pinAm1Value && !pinBm1Value) {
//    state += 1; // Если выполняется условие, наращиваем переменную state
//    lastTurn = micros();
//  }
//  if (state == -1 && !pinAm1Value && !pinBm1Value || state == -3 && pinAm1Value &&  pinBm1Value) {
//    state -= 1; // Если выполняется условие, наращиваем в минус переменную state
//    lastTurn = micros();
//  }
//  setCount(state); // Проверяем не было ли полного шага из 4 изменений сигналов (2 импульсов)
//  sei(); // Разрешаем обработку прерываний
//
//  if (pinAm1Value && pinBm1Value && state != 0) state = 0; // Если что-то пошло не так, возвращаем статус в исходное состояние
}
void Bm1()
{
    cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
//  if (micros() - lastTurn < pause) return;
  pinAm1Value = digitalRead(pinAm1);
  pinBm1Value = digitalRead(pinBm1);

//  if (pinAm1Value >0) {
//    posAm1++;
//    }
//  else {
//    posAm1--;
//    }

  if (pinBm1Value >0) {
    posBm1++;
    }
  else {
    posBm1--;
    }
sei(); // Разрешаем обработку прерываний

//  cli();
//  if (state == 1 && !pinAm1Value && !pinBm1Value || state == 3 && pinAm1Value && pinBm1Value) {
//    state += 1; // Если выполняется условие, наращиваем переменную state
//    lastTurn = micros();
//  }
//  if (state == 0 && pinAm1Value && !pinBm1Value || state == -2 && !pinAm1Value && pinBm1Value) {
//    state -= 1; // Если выполняется условие, наращиваем в минус переменную state
//    lastTurn = micros();
//  }
//  setCount(state); // Проверяем не было ли полного шага из 4 изменений сигналов (2 импульсов)
//  sei();
  
//  if (pinAm1Value && pinBm1Value && state != 0) state = 0; // Если что-то пошло не так, возвращаем статус в исходное состояние
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
  pinAm1Value = digitalRead(pinAm2);            // Получаем состояние пинов A и B
  pinBm1Value = digitalRead(pinBm2);

  if (pinAm2Value >0) {
    posAm2++;
    }
  else {
    posAm2--;
    }
sei(); // Разрешаем обработку прерываний
//  if (pinBm1Value >0) {
//    posBm2++;
//    }
//  else {
//    posBm2--;
//    }
} // Am2()
//+++++++++++++++++++++++++++++++
void Bm2()
{
    cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
    intM2Bcounter++;
//  if (micros() - lastTurn < pause) return;
  pinAm2Value = digitalRead(pinAm2);
  pinBm2Value = digitalRead(pinBm2);

//  if (pinAm2Value >0) {
//    posAm2++;
//    }
//  else {
//    posAm2--;
//    }

  if (pinBm2Value >0) {
    posBm2++;
    }
  else {
    posBm2--;
    }
  sei(); // Разрешаем обработку прерываний
} // Bm2
//+++++++++++++++++++++++++++++++
