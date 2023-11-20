#include <Arduino.h>
#include <stdlib.h>
#include "DualVNH5019MotorShield.h"
//#include "mobPlatform.h"

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>



//unsigned int getM1CurrentMilliamps()
//Returns current reading from motor 1 in milliamps.
//unsigned int getM2CurrentMilliamps()
//Returns current reading from motor 2 in milliamps.

DualVNH5019MotorShield md; // Motor Driver
//mobPlatform telega;


const  byte pinAm1 = 19;
const  byte pinBm1 = 18;
volatile long posAm1 = 0;
volatile long posBm1 = 0;

const  byte pinAm2 = 20;  //(PE4 for ATMega2560, Digital 2-pin)
const  byte pinBm2 = 21;  //(PE5 for ATMega2560, Digital 3-pin)
volatile long posAm2 = 0;
volatile long posBm2 = 0;

volatile long intM1counter = 0;
volatile long intM2counter = 0;
volatile long intM2Bcounter = 0;

String str="";
String currCommand = ""; // Текущая команда
String prevCommand = ""; // Предыдущая команда.

volatile long pause    = 50;  // Пауза для борьбы с дребезгом
volatile long lastTurn = 0;   // Переменная для хранения времени последнего изменения

volatile long count = 0;       // Счетчик оборотов
int actualcount    = 0;       // Временная переменная определяющая изменение основного счетчика

volatile int state = 0;       // Статус одного шага - от 0 до 4 в одну сторону, от 0 до -4 - в другую

volatile int pinAm1Value = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int pinBm1Value = 0;   // Переменные хранящие состояние пина, для экономии времени

volatile int pinAm2Value = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int pinBm2Value = 0;   // Переменные хранящие состояние пина, для экономии времени


#define sBufSize 32   // Размер буфера компорта arduino по умолчанию.
#define szParcel 6
byte ints[sBufSize]; // Данные, полученные по serial, - номер двигателя, скорость.
                     // Если скорость >0 - вращение в одну сторону, если скорость <0 - вращение в противоположную сторону.


volatile int m1Speed = 100;
volatile int m2Speed = 100;

volatile bool resetDone = false;

int str_len;

ros::NodeHandle  nh;
std_msgs::String str_msg;
std_msgs::String cmdVel_msg;
geometry_msgs::Vector3 gmtry;
geometry_msgs::Twist myVector;

//========== data from Serial port by USER or script
void messageCb(std_msgs::String& mobplatCommand){
  
  digitalWrite(LED_BUILTIN, HIGH-digitalRead(LED_BUILTIN));   // blink the led
  prevCommand = currCommand; // Сохраняем текущую команду, понадобится при изменении скорости.
  currCommand = mobplatCommand.data; // here we are getting currCommand from serial
  str = "The NEXT command is ";
  str.concat(currCommand);
  //write2chatter(str);
} 

//===================== Data from serial port by ROS !!!   std_msgs
//void cmd_vel_Msg(std_msgs::Float64& geometry_msg_lin_x, std_msgs::Float64& geometry_msg_ang_x){
//  float lin_x = geometry_msg_lin_x.data;
//  float ang_x = geometry_msg_ang_x.data;
//  str = "From ROS geometry_msg : ";
//  str += String(lin_x);
//  str += ", ";
//  str += String(ang_x);
//  write2chatter(str);
//  }
//===================== Data from serial port by ROS !!!   geometry_msgs/Vector3
void gmtry_msgs_Vector3(geometry_msgs::Vector3& vector){
   float x_lin = vector.x;
  
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void gmtry_msgs(geometry_msgs::Twist& myvector){
  float x_lin = myvector.linear.x;
  float y_lin = myvector.linear.y;
  float z_lin = myvector.linear.z;

  float x_ang = myvector.angular.x;
  float y_ang = myvector.angular.y;
  float z_ang = myvector.angular.z;

  str = "From ROS geometry_msg : ";
  str += "linear_x = ";
  str += String(x_lin); str += "\n";
  str += "linear_y = ";
  str += String(y_lin); str += "\n";
  str += "linear_z = ";
  str += String(z_lin); str += "\n";


  str += "angular_x = ";
  str += String(x_ang); str += "\n";
  str += "angular_y = ";
  str += String(y_ang); str += "\n";
  str += "angular_z = ";
  str += String(z_ang); str += "\n";

  write2chatter(str);
 
 // Запускаем startPlatform()
 // Или setM1Speed(x_lin);
// if (x_lin > 25) {
  m1Speed = round(x_lin);
  m2Speed = round(x_lin);
  movingOn(1);
 //}
  
  } // gmtry_msgs
  
//++++++++++++++++++++++++++++

ros::Publisher chatter("encoders", &str_msg);
ros::Subscriber<std_msgs::String> sub("mobplatform", &messageCb);

//ros::Subscriber<std_msgs::Float64> sub_ros("cmd_vel", &cmd_vel_Msg);

ros::Subscriber<geometry_msgs::Twist>sub_gmtry("cmd_vel", &gmtry_msgs);




 

//++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++



void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
}
//+++++++++++++++++++++++++++++++
volatile int diameter; // 90mm
volatile int m1A_count, m1B_Count, m2A_count, m2B_count;
volatile double m1_count, m2_count;
volatile int m1A_k, m1B_k, m2A_k, m2B_k;
volatile int  smooth_speed;

//+++++++++++++++++++++++++++++++

void setup()
{
  pinMode(pinAm1, INPUT);           // Пины в режим приема INPUT
  pinMode(pinBm1, INPUT);           // Пины в режим приема INPUT

  pinMode(pinAm2, INPUT);           // Пины в режим приема INPUT
  pinMode(pinBm2, INPUT);           // Пины в режим приема INPUT


  attachInterrupt(digitalPinToInterrupt(pinAm1), Am1, RISING);  // Настраиваем обработчик прерываний по изменению сигнала CHANGE
  attachInterrupt(digitalPinToInterrupt(pinBm1), Bm1, RISING);  // Настраиваем обработчик прерываний по изменению сигнала

  attachInterrupt(digitalPinToInterrupt(pinAm2), Am2, RISING);  // Настраиваем обработчик прерываний по изменению сигнала CHANGE
  attachInterrupt(digitalPinToInterrupt(pinBm2), Bm2, RISING);  // Настраиваем обработчик прерываний по изменению сигнала

  
  Serial.begin(115200);
  Serial.println("Dual VNH5019 Motor Shield");
  md.init();

  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(sub_gmtry);
  nh.advertise(chatter);
  mobPlatformInit();


  
}
//++++++++++++++++++++
void loop()
{
  nh.spinOnce();

//  checkReset();
  
//  if (Serial.available()) {
//    parse_command ();
//  }
//
  if (currCommand == "start") {
      //forward(1);
      movingOn(1);
  }
  if (currCommand == "stop") {
      stop();
  }
  if (currCommand == "rear") {
     // forward(-1);
      movingOn(-1);
  }

  if (currCommand == "right") {
    m1Speed = 100;
    m2Speed = 40;
    forward(1);
  }

  if (currCommand == "left") {
    m1Speed = 40;
    m2Speed = 100;
    forward(1);
  }


  if (currCommand == "moveit") {
      move_fwd(1);
      write2chatter("Move forward finished");
  }

  if (currCommand == "moveback") {
      move_fwd(-1);
      write2chatter("Move backward finished");
  }


// Reset to 0 values of ALL encoders
  if (currCommand == "reset") {
      reset_All();
      write2chatter("reset done");
      currCommand = "waiting";
  }

  if (currCommand == "getvalues"){
      getValues();
      currCommand = "waiting";
}

if (currCommand == "slowdown") {
  decSpeed();
  currCommand = prevCommand; // Только скорость поменяли.
  }
  
if (currCommand == "faster") {
  incSpeed();
  currCommand = prevCommand; // Только скорость поменяли.
  }


//  if (currCommand == "waiting") {
//      write2chatter("waiting");
//  }




//  if (actualcount != count) {     // Чтобы не загружать ненужным выводом в Serial, выводим состояние
//    actualcount = count;          // счетчика только в момент изменения
//    Serial.println(actualcount);
//  }



          
//  delay(1);

} // loop
//++++++++++++++++++++++++++++++++++++++++++++
void parse_command ()
{
    String substr, mSpeed;
    int speedVal;
    String strData = Serial.readString();
    strData.trim();
    Serial.println(strData);
    currCommand = strData;


}
//++++++++++++++++++++++++++++++++++++++++++++
// Формат команды : "M1,100" - Номер двигателя, значение скорости (6 символов - 6 байт).
void parse_command_0 ()
{
    String substr, mSpeed;
    int speedVal;

//    if (Serial.available()) {
      //byte ints[64];           // массив для численных данных, у нас 6 приводов
      byte numReaded;

      //numReaded=Serial.readBytes(ints, szParcel); // Считали данные из Serial в массив ints
      String strData = Serial.readString();
      strData.trim();
      Serial.println(strData);

      mSpeed = strData.substring(3);
      Serial.println(mSpeed);
      speedVal = mSpeed.toInt();
      
      if (strData.charAt(1)=="1"){
           md.setM1Speed(speedVal);
           m1Speed = speedVal;
           stopIfFault();
      }

      if (strData.charAt(1)=="2"){
           md.setM2Speed(speedVal);
           m2Speed = speedVal;
           stopIfFault();
      }


      //}
} // parse_command
//+++++++++++++++++++++++++

void stop()
{
    md.setM1Speed(0);
    stopIfFault();
    md.setM2Speed(0);
    stopIfFault();

}
//++++++++++++++++++++++++++
// Выводит значения энкодеров.
// Запускает движки в направлении fwd на 2с. на скорости в глобальных переменных m1Speed, m2Speed
// Останавливает движки
void forward (int fwd)
{
//+++++++++++++++++++ output encoder values ++++++++++++++
  str = "posAm1 ";
  str.concat(posAm1);
      // Serial.println(str);
  str_len = str.length() +1;
  char char_AM1_array[str_len];

// Copy it over 
  str.toCharArray(char_AM1_array, str_len);
  str_msg.data = char_AM1_array;
  chatter.publish( &str_msg );

  str = "posBm1 ";
  str.concat(posBm1);
  str_len = str.length() +1;
  char char_BM1_array[str_len];
// Copy it over 
  str.toCharArray(char_BM1_array, str_len);
  str_msg.data = char_BM1_array;
  chatter.publish( &str_msg );
//+++++++++++++++++++++
  str = "posAm2 ";
  str.concat(posAm2);
  str_len = str.length() +1;
  char char_AM2_array[str_len];
// Copy it over 
  str.toCharArray(char_AM2_array, str_len);
  str_msg.data = char_AM2_array;
  chatter.publish( &str_msg );
  

  str = "posBm2 ";
  str.concat(posBm2);
  str_len = str.length() +1;
  char char_BM2_array[str_len];
// Copy it over 
  str.toCharArray(char_BM2_array, str_len);
  str_msg.data = char_BM2_array;
  chatter.publish( &str_msg );
//+++++++++++++++++++++
  str = "intM1counter ";
  str.concat(intM1counter);
  Serial.println(str);

  str = "intM2counter ";
  str.concat(intM2counter);

  Serial.println(str);

  str = "intM2Bcounter ";
  str.concat(intM2Bcounter);

  Serial.println(str);

//+++++++++++++++++++ END OF output encoder values ++++++++++++++

//+++++++++++++ Now start moving +++++++++++++++++++++++++
  md.setM1Speed(fwd*m1Speed);
  stopIfFault();
  md.setM2Speed(fwd*m2Speed);
  stopIfFault();

  delay(2000);

  md.setM1Speed(0);
  stopIfFault();
  md.setM2Speed(0);
  stopIfFault();

  delay(2000);
} //forward
//+++++++++++++++++++++++++++++++++++++++++++++++++
// fwd - direction
// Запускает движки в направлении fwd.
// Движение постоянное, пока не будет новой команды, изменяющей скорость в m1Speed, m2Speed

void movingOn (int fwd){
  
  md.setM1Speed(fwd*m1Speed);
  stopIfFault();
  md.setM2Speed(fwd*m2Speed);
  stopIfFault();
  
  } // movingOn

void getValues()
{
    //+++++++++++++++++++
    str = "posAm1 ";
    str.concat(posAm1);
        // Serial.println(str);
    str_len = str.length() +1;
    char char_AM1_array[str_len];

    // Copy it over
    str.toCharArray(char_AM1_array, str_len);
    str_msg.data = char_AM1_array;
    chatter.publish( &str_msg );

    str = "posBm1 ";
    str.concat(posBm1);
    str_len = str.length() +1;
    char char_BM1_array[str_len];
    // Copy it over
    str.toCharArray(char_BM1_array, str_len);
    str_msg.data = char_BM1_array;
    chatter.publish( &str_msg );
    //+++++++++++++++++++++
    str = "posAm2 ";
    str.concat(posAm2);
    str_len = str.length() +1;
    char char_AM2_array[str_len];
    // Copy it over
    str.toCharArray(char_AM2_array, str_len);
    str_msg.data = char_AM2_array;
    chatter.publish( &str_msg );


    str = "posBm2 ";
    str.concat(posBm2);
    str_len = str.length() +1;
    char char_BM2_array[str_len];
    // Copy it over
    str.toCharArray(char_BM2_array, str_len);
    str_msg.data = char_BM2_array;
    chatter.publish( &str_msg );

}
//+++++++++++++++++++++++++++
void write2chatter(String mystr)
{
  
  str_len = mystr.length() +1;
  char char_MY_array[str_len];
  mystr.toCharArray(char_MY_array, str_len);
  str_msg.data = char_MY_array;
  chatter.publish( &str_msg );
  
  }
//++++++++++++
void checkReset()
{
  if(resetDone==true){
    resetDone = false;
    //reset_All();
      posAm1 = 0;
  posBm1 = 0;
  posAm2 = 0;
  posBm2 = 0;
  m1_count = 0.0;
  m2_count = 0.0;

    
    }
}

//+++++++++++++