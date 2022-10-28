#include <Arduino.h>
class mobPlatform{
  
  public:
    mobPlatform ();
    void move_fwd (byte rot); // move forward with <rot> number of rotations

    int smooth_speed;
  
  };

// Initiate mpbPlatform parameters
  mobPlatform ()
  {
    int diameter = 90; // 90mm
    int m1A_count, m1B_Count, m2A_count, m2B_count;
    double m1_count, m2_count;
    int m1A_k, m1B_k, m2A_k, m2B_k;

    
    // set the value of rotation counts
    m1A_count = 0; //
    m1A_k = 486;
    m1B_Count = 0;
    m1B_k = 487;
    m2A_count = 0;
    m2A_k = 473;
    m2B_count = 0;
    m2B_k = 473;

    smooth_speed = 40
    
    }
//+++++++++++++++++++++++++=

  void move_fwd (byte rot)
  {
      for (int i=0; i<5; i++)
      {
          md.setM1Speed(smooth_speed);
          md.setM2Speed(smooth_speed);
          delay(20);
          m1_count = (posAm1+posBm1)/(m1A_k+m1B_k);
          m2_count = (posAm2+posBm2)/(m2A_k+m2B_k);

          str = "m1_count ";
          str.concat(m1_count);
          str_len = str.length() +1;
          char char_M1_array[str_len];
        // Copy it over
          str.toCharArray(char_M1_array, str_len);
          str_msg.data = char_M1_array;
          chatter.publish( &str_msg );

          str = "m2_count ";
          str.concat(m2_count);
          str_len = str.length() +1;
          char char_M2_array[str_len];
        // Copy it over
          str.toCharArray(char_M2_array, str_len);
          str_msg.data = char_M2_array;
          chatter.publish( &str_msg );
        } //for

      stop();




  }
