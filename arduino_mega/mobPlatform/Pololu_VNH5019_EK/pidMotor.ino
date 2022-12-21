// The next line is for treating the file as cpp file
#include <Arduino.h>
// Going to regulate motor speed with PID
// Корректировку скорости вызываем, когда разница в энкодерах
// превышает GAP
int pidMspeed(int motorNumber)
{
  int mSpeed, newSpeed;
  double Kp, Ki, Kd;
  double INT, DER;
  double backlog;
  double relativeDiff; // отншение diff/GAP
  int mA_k;

  if (motorNumber == 1){
      mA_k = m1A_k;
      mSpeed = m1Speed;
    }
  else
  {
    mA_k = m2A_k;
    mSpeed = m2Speed;
    }

//  mSpeed = mSpeed*Kp + Ki*INT + Kd*DER;


// Вычисляем "отставание" в единицах "оборот колеса". Помним, что таймер может меняться,
// Соответственно это отставание происходит за время пока накапливается счетчик таймера. 
  str = "Current MxA_k is ";
  str.concat(mA_k);
  write2chatter(str);
  
  // т.к. корректировку скорости делаем по таймеру, а он раз в пол-оборота,
  // то и делить надо не на mA_k (целый), а на 0.5*mA_k
  // т.к. иногда прям перекручивает
  relativeDiff = diffAbsolute/encodersGAP;
  backlog = (double)abs(diffAbsolute - encodersGAP)/(double)(mA_k*TRC);
  str = "backlog value as double is ";
  str += String(backlog,4); str.concat(", ");
  double delta = (double)mSpeed*backlog;
  str += "speed delta value as double is ";
  str += String(delta,4); //str.concat(", ");
  write2chatter(str);
// имеем оставание колеса на backlog оборотов, соответственно, надо увеличить скорость на величину backlog т.е.

  newSpeed = round((double)mSpeed + (double)mSpeed*backlog);

  str = "newSpeed value as int is ";
  str.concat(newSpeed);
  write2chatter(str);
  return newSpeed;
}
//++++++++++++++++++++++++++++++++++++++++++++++
// Выполняет rotationNum оборотов колеса, останавливается

int makeRotation(int rotationNum)
{
  int m1A, m1B, m2A, m2B;
  int diff;
  double m_count;

  if (rotationNum == 0) {return 0;}

  if (rotationNum < 0) {
     m1Speed *= -1;
     m2Speed *= -1;
     rotationNum *= -1;
     str = "Reverse speed M1 = ";
     str.concat(String(m1Speed)); str.concat(", ");
     str += "M2 = ";
     str.concat(String(m2Speed)); //str.concat(", ");
  }

//  md.setM1Speed(m1Speed);
//  md.setM2Speed(m2Speed);
//

  m1_count = (double)posAm1/(double)m1A_k;
  m2_count = (double)posAm2/(double)m2A_k;

  if (m1_count >= m2_count){m_count=m1_count;}
  else {m_count = m2_count;}

  if(m_count >= rotationNum){ // reached the qequired number of rotations
    currCommand = "waiting";
//    cli(); // отключить глобальные прерывания
//    TIMSK1 |= (0 << OCIE1A);  // вЫключение прерываний по совпадению
//    //TIMSK1 = (1 << TOIE1);   // По переполнению
//    sei(); // включить глобальные прерывания

    m1A = posAm1;
    m1B = posBm1;
    m2A = posAm2;
    m2B = posBm2;
    diff = abs(m1A-m2A);
    diffAbsolute = diff;

    finTime = millis() - startTime;

    md.setM1Speed(0);
 //   stopIfFault();
    md.setM2Speed(0);
//    stopIfFault();

    diffRelative = 0;
    str = "Reached rotation value on first of 2 wheels with value ";
    str.concat(String(m1_count,4));
    write2chatter(str);

    str = "M1_A ";  str.concat(posAm1);  str += ", ";
    str += "M1_B ";  str.concat(posBm1); str += ", ";
    str += "M2_A ";  str.concat(posAm2); str += ", ";
    str += "M2_B ";  str.concat(posBm2); str += ", ";
    str += "finTime "; str.concat(finTime); str += ", ";
    str += "enc diff "; str.concat(diff); str += ", ";
    str += "M1speed "; str.concat(m1Speed); str += ", ";
    str += "M2speed "; str.concat(m2Speed); //str += ", ";

    write2chatter(str);
// Цикл вращений завершили, вовзращаем значения скорости в исходное.
//    if (diff <= encodersGAP) {

//    }
    Eprev = 0;
    prevT = 0;
    
    m1Speed = defaultMSpeed;
    m2Speed = defaultMSpeed;
    str = "M1speed updated on finish "; str.concat(m1Speed); str += ", ";
    str += "M2speed updated on finish "; str.concat(m2Speed);// str += ", ";

    write2chatter(str);


    return 0;
    }

  else return 1;
  
  } // makeRotation
//++++++++++++++++++++++++++++++++++++++++++++++
// Головная ф-ция PID
// ОТстающее - ускоряем, обгоняющее - замедляем

void goToPID(){
  double backlog, advanced ;
  double Derror, DprevError;
  double Kp, Ki, Kd;

    Kp = 1000.0; // В единицах скорости(сила тока)
    Ki = 1.0;
    Kd = 0.25;

    int  newSpeedAdvanced, newSpeedLag;
    int* advancedM; // Адрес скорости обгоняющего колеса
    int* advancedMxA_k;      // Адрес коэффициента MxA_k, x=[1,2]

    int *speedLagM;     // Адрес скорости отстающего колеса
    int *speedLagMxA_k; // Адрес коэффициента MxA_k, x=[1,2]
    int mxA, MxSpeed;  // encoder_A and speed

    if (posAm1 < posAm2){ // M1 is lag behind so correct M1 speed

        speedLagM = &m1Speed;
        speedLagMxA_k = &m1A_k;

        advancedM = &m2Speed; // М2 обгоняет
        advancedMxA_k = &m2A_k;       // коэфиициент MxA_k обгоняющего колеса.


    }
    if (posAm2 < posAm1) {// M2 is lag behind so correct M2 speed

        speedLagM = &m2Speed;
        speedLagMxA_k = &m2A_k;

        advancedM = &m1Speed; // М1 обгоняет
        advancedMxA_k = &m1A_k;

    }

//++++++++++++++ Отстающее колесо ускоряем

//double delitel = (double)*speedLagMxA_k*TRC;
//backlog = (double)(abs(diffAbsolute - encodersGAP))/delitel;

// int target = encodersGAP;

// Разница с "уставкой", это и есть error. error = target - measured

// Наш target - это encodersGAP
// ...
// encodersGAP набегает за 1 оборот колеса(примерно 1с) на скорости 100, сюда мы попадаем за время  deltaT


 // Если < 0 то, пора замедляться...

//  diffAbsolute = posAm1 - posAm2;
//  diffRelative = diffAbsolute - diffRelative;

if ((diffRelative <0) && !leaderIsChanged) { // slowdown the speed, make it equal to default value
  leaderIsChanged = true;
  m1Speed = defaultMSpeed;
  m2Speed = defaultMSpeed;
  
  }
if ((diffRelative > 0) && leaderIsChanged) {
  leaderIsChanged = false;
  m1Speed = defaultMSpeed;
  m2Speed = defaultMSpeed;

  }


// calculate dt
long currentT = micros();
// get microseconds
float deltaT = ((float)(currentT - prevT))/(1.0e6);
mxA = *speedLagMxA_k; // Отстающее колесо, число отсчетов на 1 оборот
MxSpeed = *speedLagM;

// E - ошибка в разнице отсчетов для М1 и М2 
float  E = (float)abs(abs(posAm1 - posAm2) - encodersGAP)*(float)1/mxA*deltaT*((float)MxSpeed/(float)defaultMSpeed);

str = "MxSpeed ";
str += String(MxSpeed); str += ", ";
str += "deltaT ";
str += String(deltaT,4); str += ", ";
str += "E ";
str.concat(String(E,4)); str.concat(", ");

write2chatter(str);
//if (E < encodersGAP) {return;}


prevT = currentT;

// Derivative
float dedt = (E - Eprev)/deltaT;

//D = (Derror - DprevError)/TRC; // 0.5c - примерное время таймера, это наше dt - интервал интегрирования
// Предыдущая разница с "уставкой"
Eprev = E;

// Integral
I = round(I + (float)E*deltaT);

// newSpeedspeedLag = round((double)(*speedLagM)*(1+backlog)) + Kd*D;
// mxA - число отсчетов энкодера за 1 оборот, а т.к. за время deltaT меньше 1 оборота (на скорости defaultMSpeed) 
// То берм отношение 1с к deltaT, также скорость не всегда defaultMSpeed, поэтому берем отношение *speedLagM к defaultMSpeed

//Kp = (double)(1/mxA)*deltaT*(double)(*speedLagM/defaultMSpeed);


// Get new speed value, corrected by PID
newSpeedLag = round((float)Kp*E + (float)Ki*I + (float)Kd*dedt); // Это типа deltaSpeed ? Управляеющее воздействие...
// Увеличиваем для отстающего
*speedLagM += newSpeedLag;

str = "*speedLagM "; 
str.concat(*speedLagM); str.concat(", ");
str += "*advancedM "; str += String(*advancedM); str.concat(", ");
str += "dedt ";
str.concat(String(dedt,4)); //str.concat(", ");

write2chatter(str);


// Это вообще убрать
//++++++++++++++ Теперь опережающее замедляем.
// advanced = (double)(abs(diffAbsolute - encodersGAP)/(*advancedMxA_k*TRC));
// newSpeedAdvanced = round((double)*advancedM*(1-advanced)) + Kd*D;
// newSpeedAdvanced = Kp*E + Ki*I + Kd*dedt;
// *advancedM = *advancedM - newSpeedAdvanced;

// ОДно из колес не изменило скорость
md.setM1Speed(m1Speed);
md.setM2Speed(m2Speed);

//backlog = 0.1234;
str = "dedt ";
str.concat(String(dedt,4)); str.concat(", ");

str += "speedLagMxA_k ";
str += String(*speedLagMxA_k); str.concat(", ");
str += "advancedMxA_k ";
str += String(*advancedMxA_k); str.concat(", ");

write2chatter(str);

//str = "Derror ";
//str += String(Derror); str.concat(", ");
//
//str += "DprevError ";
//str += String(DprevError); str.concat(", ");
//
//str += "D = ";
//str.concat(String(D,4)); str.concat(", ");
//write2chatter(str);
//
str = "M1speed "; str.concat(m1Speed); str += ", ";
str += "M2speed "; str.concat(m2Speed);// str += ", ";

write2chatter(str);

} // goToPID
