#include <Arduino.h>
#include <stdlib.h>
#include "DualVNH5019MotorShield.h"
//#include "mobPlatform.h"

#include <ros.h>
#include <std_msgs/String.h>



//unsigned int getM1CurrentMilliamps()
//Returns current reading from motor 1 in milliamps.
//unsigned int getM2CurrentMilliamps()
//Returns current reading from motor 2 in milliamps.

DualVNH5019MotorShield md;
//mobPlatform telega;


const  byte pinAm1 = 19;
const  byte pinBm1 = 18;
volatile int posAm1 = 0;
volatile int posBm1 = 0;

const  byte pinAm2 = 20;  //(PE4 for ATMega2560, Digital 2-pin)
const  byte pinBm2 = 21;  //(PE5 for ATMega2560, Digital 3-pin)
volatile int posAm2 = 0;
volatile int posBm2 = 0;

volatile int intM1counter = 0;
volatile int intM2counter = 0;
volatile int intM2Bcounter = 0;

String str="";
volatile String currCommand = "";

volatile long pause    = 50;  // Пауза для борьбы с дребезгом
volatile long lastTurn = 0;   // Переменная для хранения времени последнего изменения

volatile int count = 0;       // Счетчик оборотов
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

int str_len;

ros::NodeHandle  nh;
std_msgs::String str_msg;
ros::Publisher chatter("encoders", &str_msg);


//========== data from Serial port
void messageCb(std_msgs::String& mobplatCommand){
  
  digitalWrite(LED_BUILTIN, HIGH-digitalRead(LED_BUILTIN));   // blink the led
  currCommand = mobplatCommand.data; // here we are getting currCommand from serial
}

ros::Subscriber<std_msgs::String> sub("mobplatform", &messageCb );





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
  nh.advertise(chatter);
  mobPlatformInit();


  
}
//++++++++++++++++++++
void loop()
{
  nh.spinOnce();

  
//  if (Serial.available()) {
//    parse_command ();
//  }
//
  if (currCommand == "start") {
      forward();
  }
  if (currCommand == "stop") {
      stop();
  }

  if (currCommand == "moveit") {
      move_fwd(1);
      write2chatter("Move forward");
  }

  if (currCommand == "moveback") {
      move_fwd(-1);
      write2chatter("Move backward");
  }


// Reset to 0 values of ALL encoders
  if (currCommand == "reset") {
      reset_All();
      write2chatter("reset done");
      currCommand = "waiting";
  }

  if (currCommand == "waiting") {
      write2chatter("waiting");
  }




//  if (actualcount != count) {     // Чтобы не загружать ненужным выводом в Serial, выводим состояние
//    actualcount = count;          // счетчика только в момент изменения
//    Serial.println(actualcount);
//  }



          
  delay(1);

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
//    switch (strData) {
//
//        case "start":
//
//            forward();
//        break;
//
//        case "stop":
//            stop();
//        break;
//
//    default:
//        message = "Wrong data !!!";
//        //strcpy(buf, message.c_str());
//        Serial.print(message);
//
//
//    }


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
void forward ()
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



  md.setM1Speed(m1Speed);
  stopIfFault();
  md.setM2Speed(m2Speed);
  stopIfFault();

  delay(2000);

  md.setM1Speed(0);
  stopIfFault();
  md.setM2Speed(0);
  stopIfFault();

  delay(2000);
} //forward
//++++++++++

void write2chatter(String mystr)
{
  
  str_len = mystr.length() +1;
  char char_MY_array[str_len];
  mystr.toCharArray(char_MY_array, str_len);
  str_msg.data = char_MY_array;
  chatter.publish( &str_msg );
  
  }
//++++++++++++

//for (int i = 0; i <= 400; i++)
//{
//  md.setM1Speed(i);
//  stopIfFault();
//  if (abs(i)%200 == 100)
//  {
//    Serial.print("M1 current [0;400]: ");
//    Serial.println(md.getM1CurrentMilliamps());
//  }
//  delay(2);
//}

//for (int i = 400; i >= -400; i--)
//{
//  md.setM1Speed(i);
//  stopIfFault();
//  if (abs(i)%200 == 100)
//  {
//    Serial.print("M1 current [400; -400]: ");
//    Serial.println(md.getM1CurrentMilliamps());
//  }
//  delay(2);
//}

//for (int i = -400; i <= 0; i++)
//{
//  md.setM1Speed(i);
//  stopIfFault();
//  if (abs(i)%200 == 100)
//  {
//    Serial.print("M1 current [-400; 0]: ");
//    Serial.println(md.getM1CurrentMilliamps());
//  }
//  delay(2);
//}

//for (int i = 0; i <= 400; i++)
//{
//  md.setM2Speed(i);
//  stopIfFault();
//  if (abs(i)%200 == 100)
//  {
//    Serial.print("M2 current [0;400]: ");
//    Serial.println(md.getM2CurrentMilliamps());
//  }
//  delay(2);
//}

//for (int i = 400; i >= -400; i--)
//{
//  md.setM2Speed(i);
//  stopIfFault();
//  if (abs(i)%200 == 100)
//  {
//    Serial.print("M2 current [400; -400]: ");
//    Serial.println(md.getM2CurrentMilliamps());
//  }
//  delay(2);
//}

//for (int i = -400; i <= 0; i++)
//{
//  md.setM2Speed(i);
//  stopIfFault();
//  if (abs(i)%200 == 100)
//  {
//    Serial.print("M2 current [-400; 0]: ");
//    Serial.println(md.getM2CurrentMilliamps());
//  }
//  delay(2);
//}
