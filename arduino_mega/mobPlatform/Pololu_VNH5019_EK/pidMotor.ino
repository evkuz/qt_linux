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
backlog = diffAbsolute/mA_k;

// имеем оставание колеса на backlog оборотов, соответственно, надо увеличить скорость на величину backlog т.е.
  newSpeed = mSpeed + (double)mSpeed*backlog;

  return newSpeed;
}
//++++++++++++++++++++++++++++++++++++++++++++++
// Выполняет rotationNum оборотов колеса, останавливается

int makeRotation(int rotationNum)
{
  int m1A, m1B, m2A, m2B;
  int diff;
//  md.setM1Speed(smooth_speed);
//  md.setM2Speed(smooth_speed);
//  m1Speed = 100;
//  m2Speed = 100;

if (rotationNum <=0) {
//  str = "reverse rotations";
//   write2chatter(str);
   m1Speed *= -1;
   m2Speed *= -1;
   rotationNum *= -1;
//  return 0;
}

  md.setM1Speed(m1Speed);
  md.setM2Speed(m2Speed);


  m1_count = (double)posAm1/(double)m1A_k;
  m2_count = (double)posAm2/(double)m2A_k;

  if(m1_count >= rotationNum){ // reached the qequired number of rotations
    m1A = posAm1;
    m1B = posBm1;
    m2A = posAm2;
    m2B = posBm2;
    diff = m1A-m2A;

    finTime = millis() - startTime;
    stop();
    diffRelative = 0;
    str = "Reached rotation value on first of 2 wheels with value ";
    str.concat(String(m1_count,4));
    write2chatter(str);

    str = "M1_A ";  str.concat(posAm1);  str += ", ";
    str += "M1_B ";  str.concat(posBm1); str += ", ";
    str += "M2_A ";  str.concat(posAm2); str += ", ";
    str += "M2_B ";  str.concat(posBm2); str += ", ";
    str+= "finTime "; str.concat(finTime); str += ", ";
    str+= "enc diff "; str.concat(diff); str += ", ";

    write2chatter(str);
//    str_len = str.length() + 1;
//    char int2_array[str_len];
//    str.toCharArray(int2_array, str_len);
//    str_msg.data = int2_array;
//    chatter.publish( &str_msg );
    
    

    return 0;
    }

  else return 1;
  
  } // makeRotation
//++++++++++++++++++++++++++++++++++++++++++++++
  
