
// Initiate mpbPlatform parameters
 
void mobPlatformInit()
  {
    diameter = 90; // 90mm
    
    // set the value of rotation counts
    m1A_count = 0; //
    m1A_k = 486;
    m1B_Count = 0;
    m1B_k = 487;
    m2A_count = 0;
    m2A_k = 473;
    m2B_count = 0;
    m2B_k = 473;

    smooth_speed = 70;
    
    }
//+++++++++++++++++++++++++=

  void move_fwd (byte rot)
  {
      for (int i=0; i<350; i++)
      {
          md.setM1Speed(smooth_speed);
          md.setM2Speed(smooth_speed);
          
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
//            str = "Reached 1.00 value";
//            str_len = str.length() +1;
//            str = "Finished with i value ";
//            str = str.concat(i);
//            str_len = str.length() + 1;
//            char int_array[str_len];
//            str.toCharArray(int_array, str_len);
//            str_msg.data = int_array;
//            chatter.publish( &str_msg );
            
            break;
            }
        } //for

     double dval = 0.3764;
     str = String(dval,4);
     str_len = str.length() +1;
     char dbl_array[str_len];
     str.toCharArray(dbl_array, str_len);
     str_msg.data = dbl_array;
     chatter.publish( &str_msg );
     
     currCommand = "stop";




  }