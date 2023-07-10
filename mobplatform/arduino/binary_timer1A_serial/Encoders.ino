#include <Arduino.h>


// M2 - 480 на 1 оборот
// M1 - 476 на 1 оборот


void mobPlatformInit()
  {
    diameter = 90; // 90mm
    
    // set the value of rotation counts
    m1A_count = 0; //
    m1A_k = 475; //486  473
    m1B_Count = 0;
    m1B_k = 473; //487
    m2A_count = 0;
    m2A_k = 490; // 473
    m2B_count = 0;
    m2B_k = 473;

  //  smooth_speed = 0.7*defaultMSpeed;
    
    }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
void Am1()
{
  cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
//  if (micros() - lastTurn < pause) return;  // Если с момента последнего изменения состояния не прошло
//  // достаточно времени - выходим из прерывания
  pinAm1Value = digitalRead(pinAm1);            // Получаем состояние пинов A и B
//  pinBm1Value = digitalRead(pinBm1);

//intM1counter++;
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
//void setCount(int state) {          // Устанавливаем значение счетчика
//  if (state == 4 || state == -4) {  // Если переменная state приняла заданное значение приращения
//    count += (int)(state / 4);      // Увеличиваем/уменьшаем счетчик
//    lastTurn = micros();            // Запоминаем последнее изменение
//  }
//}
//++++++++++++++++++++++++++++++++++++++++++
void Am2()
{
    cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
//  if (micros() - lastTurn < pause) return;  // Если с момента последнего изменения состояния не прошло
//  // достаточно времени - выходим из прерывания
//intM2counter++;
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
//    intM2Bcounter++;
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
  posAm1_prev = 0;
  posAm2_prev = 0;
  m1Speed = defaultMSpeed;
  m2Speed = defaultMSpeed;
  startFlag = false;
  pidFlag = false;

  Eintegral = 0;
  prevT = 0;
  Eprev = 0;

  }
//++++++++++++++++++++++++++++++++
// Инициализация структуры
void struct_init(){
  
  data.A1_Enc = 0;
  data.A2_Enc = 0;

  data.M1_Speed = 0;
  data.M2_Speed = 0;
  data.time_ms = 0;
  data.deltaT = 0.0; // 1234.5678; 
  data.dedt = 0.0; //3.5689;  
  data.E = 0;
  data.Eprev = 0;
  
  data.lagSpeed_ptr = &m1Speed;
  data.fwdSpeed_ptr = &m2Speed;
  data.M1Speed_ptr = &m1Speed;
  data.M2Speed_ptr = &m2Speed;

  data.Integral = 0.0;
  //str = "stopped ";
  currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));

  }
//++++++++++++++++++++++++++++++++
void pid()
{
/*
 * Параметры для вычисления ПИД-коэффициентов
 * - Время срабатывания таймера. Чем больше, тем больше разница в показаниях энкодеров
 * - Разница в показаниях энкодеров
 * - Количество отсчетов за 1 оборот колеса.
 * - Число осчетов за 1 цикл таймера при штатной скорости
  
  
  */
//cli();
// u(t) = round((float)Kp*E(t) + (float)Ki*I + (float)Kd*dedt); // Управляеющее воздействие...  

  float Kp = 0.06;
  float Kd = 0.009;
  float Ki = 0.015;
  float u;

  int E; // Ошибка - отличие фактического от целевого.

  float  Er; // Вычисляем

  int diffLimit = 100; // Если больше этого, включаем ПИД

  volatile int mxA, MxSpeed;  // encoder_A and speed
  
  int delta, delta0;  // Разница между энкодерами
  int delta_A1, delta_A2; // разница с прошлым значением
   
  E = posAm1 - posAm2;

  int speedA1 = m1Speed;
  int speedA2 = m2Speed;
  
  if (E < 0) {lagmSpeed = &m1Speed; fwdmSpeed = &m2Speed;}
  if (E > 0) {lagmSpeed = &m2Speed; fwdmSpeed = &m1Speed;}

  delta0 = abs(E);
//  delta = abs(delta0 - encodersGAP);

  

// Get new speed value, corrected by PID, as u(t)
// Управляющее воздействие в нашем случае - скорость.

// newSpeedLag = round((float)Kp*E + (float)Ki*I + (float)Kd*dedt); // Это типа deltaSpeed ? Управляеющее воздействие...

// calculate dt
unsigned long currentT = micros();

// Микросекунды делим на 1.0e6, получаем секунды.
// Иногда значения получаются отрицательные... счетчик переполнился ? 
float deltaT = ((float)(currentT - prevT))/(1.0e6);

if (deltaT == 0) {deltaT = 0.005;}



prevT = currentT;
int eprev = Eprev;

// Derivative
int gradE = E - Eprev; // Наблюдаем изменение ошибки

float dedt = float((gradE)/deltaT);

// Integral 
Eintegral = Eintegral + E*deltaT;

Eprev = E;

float P = (float)Kp*E;
float I = (float)Ki*Eintegral;
float D = Kd*dedt;


u = abs(Kp*E + I + Kd*dedt);
float calcU = u;

//u = u/2; // Т.к. "в воздухе", то скорость в 2 раза ниже реальной, поэтому и u сделаем меньше в 2 раза. т.к. на оба колеса, то еще уполовиним.



if (0<u<1) {u=1.0;}
if (-1<u<0) {u = -1.0;}



*lagmSpeed = *lagmSpeed + round(u/2);
*fwdmSpeed = *fwdmSpeed - round(u);

*lagmSpeed = constrain(*lagmSpeed, speedBottomLimit, speedTopLimit);
*fwdmSpeed = constrain(*fwdmSpeed, speedBottomLimit, speedTopLimit); 

//  if (abs(*lagmSpeed) > speedTopLimit || abs(*fwdmSpeed) > speedTopLimit) {
//  m1Speed = defaultMSpeed;
//  m2Speed = defaultMSpeed;
//  currCommand = "pid";
//  //getValues(deltaT, dedt);
//  }
//
//if (m1Speed < speedBottomLimit) {m1Speed = speedBottomLimit; } //m2Speed = 0.8*m2Speed;
//if (m2Speed < speedBottomLimit) {m2Speed = speedBottomLimit; } //m1Speed = 0.8*m1Speed;  
md.setM1Speed(m1Speed);
md.setM2Speed(m2Speed);


//  str = "posAm1="; 
//  str += String(posAm1); str += ", ";
//  str += "posAm2=" ; 
//  str += String(posAm2); str += ", ";
//
//
//  str += "m1Speed="; 
//  str += String(speedA1); str += ", ";
//  
//  str += "m2Speed="; 
//  str += String(speedA2); str += ", "; // str += "\n";
//
//  str += "E ="; 
//  str += String(E); str += ", "; // str += "\n";
//
//  str += "alfa = ";
//  str += String(alfa,4); str += ", "; 
//
//  str += "beta = ";
//  str += String(beta,4); str += ", "; 
//
//  str += "Kp = ";
//  str += String(Kp,4); str += ", "; 
//
//  str += "Eprev = ";
//  str += String(eprev); str += ", "; 
//
//
//  str += "deltaT = ";
//  str += String(deltaT,4); str += ", "; 
//
//
//  str += "dedt = ";
//  str += String(dedt,4); str += ", "; 
//
//  str += "dltE = ";
//  str += String(gradE); str += ", "; 
//
//  str += "u = ";
//  str += String(u,4); str += "\n"; 
//
//  str += "*lagmSpeed = ";
//  str += String(*lagmSpeed); str += ", "; 
//  
//  str += "*fwdmSpeed = "; 
//  str += String(*fwdmSpeed); str += ", "; 
//
//  str += "P = ";
//  str += String(P,4); str += " "; 
//
//  str += "I = ";
//  str += String(I,4); str += " "; 
//
//  str += "D = ";
//  str += String(D,4); str += " "; 
//
//  str += "calcU = ";
//  str += String(calcU,4); str += "\n"; 
//  Serial.println(String(data.deltaT,4));



//  Serial.println(str);


  data.A1_Enc = posAm1;
  data.A2_Enc = posAm2;
  data.diff = E;
  data.diff_vector = 555;

  data.M1_Speed = speedA1;
  data.M2_Speed = speedA2;
  data.time_ms = millis();
  data.deltaT = deltaT; // 1234.5678; 
  data.dedt = dedt; //3.5689;  
  data.E = E;
  data.Eprev = eprev;

  data.lagSpeed_ptr = m1Speed; // Значение после ПИД
  data.fwdSpeed_ptr = m2Speed; // Значение после ПИД
  data.M1Speed_ptr = &m1Speed;
  data.M2Speed_ptr = &m2Speed;

  data.Integral = I;
  
  currCommand = "pid";
  
  currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));
  data.timestamp = movingTime;

//  Serial.println("PID !!!");
//  Serial.flush();


 
  Serial.write((byte*)&data, sizeof(data));
  Serial.flush();
//  sei();
  
  
  

 }//pid()

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MoveIfStopped()
{
  int delta_A1, delta_A2; // разница с прошлым значением  
  float f1Speed = 0.0;
  float f2Speed = 0.0; // вещественное значение Скорости
  
  
  delta_A1 = posAm1 - posAm1_prev;
  delta_A2 = posAm2 - posAm2_prev;

  while (((posAm1 - posAm1_prev)<10) || ((posAm2 - posAm2_prev) < 10)) // По "ИЛИ" должен включаться ПИД
  {


    // Если не сдвинулся, то увеличиваем скорость
    f1Speed += 0.5; // 1.1*m1Speed;
    f2Speed += 0.5; // 1.1*m2Speed;
    
    m1Speed = defaultMSpeed + round(f1Speed);
    m2Speed = defaultMSpeed + round(f2Speed);
    
    md.setM1Speed(m1Speed);
    md.setM2Speed(m2Speed);

    posAm1_prev = posAm1;
    posAm2_prev = posAm2;

    delay(200);
//    delta_A1 = posAm1 - posAm1_prev;
//    delta_A2 = posAm2 - posAm2_prev;

    
  }
  startFlag=false; // Уже стартанули, больше сюда не надо.
  
}//MoveIfStopped()
//++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
