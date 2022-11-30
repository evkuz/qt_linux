// The next line is for treating the file as cpp file
#include <Arduino.h>
// Going to regulate motor speed with PID
int pidMspeed(int motorNumber)
{
  int mSpeed, newSpeed;
  double Kp, Ki, Kd;
  double INT, DER;
  double backlog;
  //diffAbsolute
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
  str = "Current mxA_k is ";
  str.concat(mA_k);
  write2chatter(str);
  
  backlog = (double)abs(diffAbsolute)/(double)mA_k;
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
//  md.setM1Speed(smooth_speed);
//  md.setM2Speed(smooth_speed);
//  m1Speed = 100;
//  m2Speed = 100;
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

  md.setM1Speed(m1Speed);
  md.setM2Speed(m2Speed);


  m1_count = (double)posAm1/(double)m1A_k;
  m2_count = (double)posAm2/(double)m2A_k;

  if (m1_count >= m2_count){m_count=m1_count}
  else {m_count = m2_count }

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

    write2chatter(str);

    if (diff <= encodersGAP) {
        m1Speed = 100;
        m2Speed = 100;

        md.setM1Speed(m1Speed);
        md.setM2Speed(m2Speed);

    }
    
    

    return 0;
    }

  else return 1;
  
  } // makeRotation
//++++++++++++++++++++++++++++++++++++++++++++++
  
