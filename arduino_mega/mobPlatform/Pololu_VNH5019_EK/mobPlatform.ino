// Platform basic control functions 



// Initiate mpbPlatform parameters
 
void mobPlatformInit()
  {
    diameter = 90; // 90mm
    
    // set the value of rotation counts
    m1A_count = 0; //
    m1A_k = 473; //486
    m1B_Count = 0;
    m1B_k = 473; //487
    m2A_count = 0;
    m2A_k = 473;
    m2B_count = 0;
    m2B_k = 473;

    smooth_speed = 70;
    
    }
//+++++++++++++++++++++++++=
//fwd = 1 вперед
//fwd = -1 назад
void move_fwd (int fwd)
{
  posAm1 = 0;
  posBm1 = 0;
  posAm2 = 0;
  posBm2 = 0;

      for (int i=0; i<350; i++)
      {
          md.setM1Speed(fwd*smooth_speed);
          md.setM2Speed(fwd*smooth_speed);
          
          delay(5);
          
          m1_count = (double)(posAm1+posBm1)/(double)(m1A_k+m1B_k);
          m2_count = (double)(posAm2+posBm2)/(double)(m2A_k+m2B_k);

          str = "m1_count ";
          str.concat(String(m1_count,4));
          str_len = str.length() +1;
          char char_M1_array[str_len];
        // Copy it over
          str.toCharArray(char_M1_array, str_len);
          str_msg.data = char_M1_array;
          chatter.publish( &str_msg );

          str = "m2_count ";
          str.concat(String(m2_count,4));
          str_len = str.length() +1;
          char char_M2_array[str_len];
        // Copy it over
          str.toCharArray(char_M2_array, str_len);
          str_msg.data = char_M2_array;
          chatter.publish( &str_msg );

          if ((m1_count >1.00) || (m2_count > 1.00))
          {
            str = "Reached 1.00 value on one of 2 wheels with i value ";
            //str_len = str.length() +1;
            //str += "Finished with i value ";
            str.concat(i);
            str_len = str.length() + 1;
            char int_array[str_len];
            str.toCharArray(int_array, str_len);
            str_msg.data = int_array;
            chatter.publish( &str_msg );
            break;
            }
        } //for

//     double dval = 0.3764;
//     str = String(dval,4);
//     str_len = str.length() +1;
//     char dbl_array[str_len];
//     str.toCharArray(dbl_array, str_len);
//     str_msg.data = dbl_array;
//     chatter.publish( &str_msg );


//     currCommand = "stop";




  } //move_fwd
//+++++++++++++++++++++++++++++++++++++
// Уменьшаем скорость каждого двигателя на 20%
// Нужно где-то взять текущее значение fwd
void decSpeed(){
//  str = "Current speed BEFORE SlowDown is ";
//  
//  str.concat("m1="); str.concat(m1Speed); str.concat(", ");
//  str.concat("m2="); str.concat(m2Speed); 
//  str_len = str.length() +1;
//  char str_array[str_len];
//  str.toCharArray(str_array, str_len);
//  str_msg.data = str_array;
//  chatter.publish( &str_msg );
//  
//  int newM1Speed = m1Speed*0.8;
//  int newM2Speed = m2Speed*0.8;

  m1Speed = m1Speed*0.8; //newM1Speed;
  m2Speed = m2Speed*0.8; //newM2Speed;

  str = "Current speed AFTER SlowDown is ";
  str.concat("m1="); str.concat(m1Speed); str.concat(", ");
  str.concat("m2="); str.concat(m2Speed); 
  str_len = str.length() +1;
  char str2_array[str_len];
  str.toCharArray(str2_array, str_len);
  str_msg.data = str2_array;
  chatter.publish( &str_msg );
 
  }// decSpeed  
//+++++++++++++++++++++++++++++++++++++
// Увеличиваем скорость каждого двигателя на 20%
// Нужно где-то взять текущее значение fwd
void incSpeed(){
//  str = "Current speed BEFORE accelerate is ";
//  
//  str.concat("m1="); str.concat(m1Speed); str.concat(", ");
//  str.concat("m2="); str.concat(m2Speed); 
//  str_len = str.length() +1;
//  char str_array[str_len];
//  str.toCharArray(str_array, str_len);
//  str_msg.data = str_array;
//  chatter.publish( &str_msg );
  
//  int newM1Speed = m1Speed*1.2;
//  int newM2Speed = m2Speed*1.2;

  m1Speed = m1Speed*1.2; //newM1Speed;
  m2Speed = m2Speed*1.2; //newM2Speed;

  str = "Current speed AFTER accelerate is ";
  str.concat("m1="); str.concat(m1Speed); str.concat(", ");
  str.concat("m2="); str.concat(m2Speed); 
  str_len = str.length() +1;
  char str2_array[str_len];
  str.toCharArray(str2_array, str_len);
  str_msg.data = str2_array;
  chatter.publish( &str_msg );
 
  }// incSpeed    
  //+++++++++++++++++++++++++++++++++++++
