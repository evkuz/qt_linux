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

  // Скорость не  меняем. Нужно для настройки равномерного вращения колес.
//  m1Speed = speedBottomLimit; //defaultMSpeed;
//  m2Speed = speedBottomLimit; //defaultMSpeed;
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
  data.diff = 0;          // 2 bytes
  data.diff_vector = 0;   // 2 bytes


  data.M1_Speed = 0;
  data.M2_Speed = 0;
  data.time_ms = 0;
  data.deltaT = 1.0; // 1234.5678; 
  data.dedt = 1.0; //3.5689;  
  data.E = 0;
  data.Eprev = 0;
  
  data.lagSpeed_ptr = &m1Speed;
  data.fwdSpeed_ptr = &m2Speed;
  data.M1Speed_start = &m1Speed;
  data.M2Speed_start = &m2Speed;

  data.Integral = 0.0;
  //str = "stopped ";
  currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));
  data.timestamp = millis();
 
  data.Proportional = Kp;
  data.Integral_k = Ki;
  data.Derivative = Kd;

  str = "каждый охотник желает знать где сидит фазан";
  str.toCharArray(data.mytext, sizeof(data.mytext));
  

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

//  float Kp = 0.0235;
//  float Kd = 0.0275;
//  float Ki = 0.0147;

  int* fwdmSpeed; // Скорость опережающего
  int* lagmSpeed; // Скорость отстающего колеса

  float u;

  int E; // Ошибка - отличие фактического от целевого.

  float  Er; // Вычисляем

  int diffLimit = 100; // Если больше этого, включаем ПИД

  volatile int mxA, MxSpeed;  // encoder_A and speed
  
  int delta, delta0;  // Разница между энкодерами
  int delta_A1, delta_A2; // разница с прошлым значением

  Am1();
  Am2();

  long A1 = posAm1;
  long A2 = posAm2;
 
  E = A1 - A2;
  

  int speedA1 = m1Speed;
  int speedA2 = m2Speed;
  
  if (E < 0) {lagmSpeed = &m1Speed; fwdmSpeed = &m2Speed;}
  if (E > 0) {lagmSpeed = &m2Speed; fwdmSpeed = &m1Speed;}

  delta0 = abs(E);

// Get new speed value, corrected by PID, as u(t)
// Управляющее воздействие в нашем случае - скорость.

// newSpeedLag = round((float)Kp*E + (float)Ki*I + (float)Kd*dedt); // Это типа deltaSpeed ? Управляеющее воздействие...

// calculate dt
unsigned long currentT = millis(); //micros();

// Микросекунды делим на 1.0e6, получаем секунды.
// Иногда значения получаются отрицательные... счетчик переполнился ? 
float deltaT = ((float)(currentT - prevT))/(1.0e3);

if (deltaT == 0) {deltaT = 1.9999;}



prevT = currentT;
int eprev = Eprev;

// Derivative
int gradE = E - Eprev; // Наблюдаем изменение ошибки

// Вот тут можно добавить, что если gradE < encodersGAP, это значит, что ошибка совсем мала... Но если так, то мы сюда вообще не должны попадать.
// А вот и нет. Это не сама ошибка, а РАЗНИЦА  с прошлой ошибкой

//if (gradE < encodersGAP){
//  gradE = 0;
//  }

float dedt = float((gradE)/deltaT);

// Integral 
Eintegral = Eintegral + E*deltaT; // Тут время в мкс. Может имеет смысл x1000 ?

Eprev = E;

float P = (float)Kp*E;
float I = (float)Ki*Eintegral;
float D = Kd*dedt;

if (I > 10) {I = 10;}
if (I < -10) {I = -10;}

u = abs(Kp*E + I + Kd*dedt);
float calcU = u;

//u = u/2; // Т.к. "в воздухе", то скорость в 2 раза ниже реальной, поэтому и u сделаем меньше в 2 раза. т.к. на оба колеса, то еще уполовиним.



if (0<u<1) {u=1.0;}
if (-1<u<0) {u = -1.0;}


int lessSpeed = *lagmSpeed;
int gtSpeed = *fwdmSpeed;


lessSpeed += round(u);
gtSpeed -= round(u);
// Constrains a number to be within a range.

if (lessSpeed < speedBottomLimit) {lessSpeed = speedBottomLimit;}
if (gtSpeed > speedTopLimit) {gtSpeed = speedTopLimit;}

*lagmSpeed = lessSpeed;//constrain(lessSpeed, speedBottomLimit, speedTopLimit);
*fwdmSpeed = gtSpeed;//constrain(gtSpeed, speedBottomLimit, speedTopLimit); 

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

  data.A1_Enc = A1;
  data.A2_Enc = A2;
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
  data.M1Speed_start = lagmSpeed; //&m1Speed;
  data.M2Speed_start = fwdmSpeed;//&m2Speed;

  data.Integral = I;

  data.Proportional = P; // P имеет тип double(!) -это 8 байт. Меняем на float
  data.Integral_k = Ki;
  data.Derivative = D;
  
  currCommand = "pid";
  
  currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));
  data.timestamp = movingTime;

  str = "u = ";
  str += String(u,4); str += "  inside pid() ";
  str += "Am1 = "; 
  str += String(A1); 
  str.toCharArray(data.mytext, sizeof(data.mytext));
  
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
//+++ определение скорости "отрыва от земли" - сила тока, при которой каждое из колес начинает вращаться в положении "на вису".
//++++ Данная ф-ция используется при отладке/пусконаладке для определения скорости. Далее, в режиме "на земле", эти значения 
//++++ присваиваются переменным m[12]LightSpeed

void findStartCurrent(){
  reset_All();
  int posA1;
  int posA2;

  //startM[12]Speed - какие-то прошлые значения. Могут быть как больше, так и меньше актуальных на данный момент.
  m1Speed=15;// startM1Speed - 75; // На земле (-40)
  m2Speed=15;// startM2Speed - 75; 

       
  md.setM1Speed(m1Speed); //defaultMSpeed
  md.setM2Speed(m2Speed);
  delay(200);
  
  float f1Speed = 0.0;
  float f2Speed = 0.0; // вещественное значение Скорости
  posA1 = posAm1;
  posA2 = posAm2;
        
  bool m1Started = false;
  bool m2Started = false;
  
  while (!m1Started || !m2Started) 
  //((posA1 < 200) || (posA2 < 200))
  {
    delay(5);
   // Если не сдвинулся, то увеличиваем скорость
   if (!m1Started) {f1Speed += 0.2; m1Speed += round(f1Speed); md.setM1Speed(m1Speed);}// 1.1*m1Speed;
   if (!m2Started) {f2Speed += 0.2; m2Speed += round(f2Speed); md.setM2Speed(m2Speed);}// 1.1*m2Speed;
           
    delay(400);
    if (posAm1 > 200) {posAm1 = -1000; m1Started = true; startM1Speed = m1Speed;}; //  str = "M1 Started at " ; str += String(m1Speed); Serial.println(str);}
    if (posAm2 > 200) {posAm2 = -1000; m2Started = true; startM2Speed = m2Speed;}; //  str = "M2 Started at " ; str += String(m2Speed); Serial.println(str);}
    posA1 = posAm1;
    posA2 = posAm2;
  
  }

 // movingTime = millis(); // Фиксируем время начала движения
//  currCommand = "calibration";
  
  m1LightSpeed = startM1Speed*0.85;
  m2LightSpeed = startM2Speed*0.85;

  // Задаем значений *prev для последующей фиксации остановки.
    posAm1_prev = posA1;
    posAm2_prev = posA2;
//  currCommand = "moving"; // Меняем на мувинг, чтобы начался отсчет времени elapsedTime
  

  } // findStartCurrent()
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
