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

DualVNH5019MotorShield md; // Motor Driver
//mobPlatform telega;


const  byte pinAm1 = 19;
const  byte pinBm1 = 18;

const  int defaultMSpeed = 100;
//const  int defaultM2Speed = 100;

volatile long posAm1 = 0;
volatile long posBm1 = 0;

const  byte pinAm2 = 20;  //(PE4 for ATMega2560, Digital 2-pin)
const  byte pinBm2 = 21;  //(PE5 for ATMega2560, Digital 3-pin)
volatile long posAm2 = 0;
volatile long posBm2 = 0;

volatile long intM1counter = 0;
volatile long intM2counter = 0;
volatile long intM2Bcounter = 0;

volatile long diffAbsolute = 0;
volatile long diffRelative = 0;

volatile unsigned long startTime;
volatile unsigned long finTime;

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


volatile int m1Speed = defaultMSpeed;
volatile int m2Speed = defaultMSpeed;
bool advancingWheel_1; // Ведущее колесо, если true - ведущее 1-е колесо, иначе - 2е колесо.

volatile bool resetDone = false;

int str_len;

ros::NodeHandle  nh;
std_msgs::String str_msg;
ros::Publisher chatter("encoders", &str_msg);

String commands[3] = {"start", "stop", "mkrotation"};

//========== data from Serial port
void messageCb(std_msgs::String& mobplatCommand){
  
  digitalWrite(LED_BUILTIN, HIGH-digitalRead(LED_BUILTIN));   // blink the led
  prevCommand = currCommand; // Сохраняем текущую команду, понадобится при изменении скорости.
  currCommand = mobplatCommand.data; // here we are getting currCommand from serial
  str = "The NEXT command is ";
  str.concat(currCommand);
  write2chatter(str);
  // Взять аргументы у команды
}  

ros::Subscriber<std_msgs::String> sub("mobplatform", &messageCb);

int numRot; // Число оборотов колеса



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
volatile int m1A_k, m1B_k, m2A_k, m2B_k; // число отсчетов энкодера на 1 оборот колеса. Здесь k - коэффициент
volatile int  smooth_speed;

volatile int encodersGAP;  // Это порог разницы в показаниях энкодеров, при превышении - запускаем ПИД

//+++++++++++++++++++++++++++++++++++++++++ PID variables +++++++++++++++++++
// volatile double Kp, Ki, Kd;
volatile double P, I, D;
const int timerPerRotation = 2; // Сколько раз срабатывает таймер за 1 оборот колеса
volatile double TRC = (double)1/timerPerRotation; // timer/rotation coefficient == dt т.к. таймер за 1 сек.

volatile long prevT; // for delta t counting
volatile float Eprev; // for delta Error counting

boolean leaderIsChanged; // Флаг смены отстающего колеса на ведущее. Если М1_А > M2_A leaderIsChanged == false

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

//+++++++++++++++ set up timer 1
    // инициализация Timer1

    cli(); // отключить глобальные прерывания
    TCCR1A = 0; // установить регистры в 0
    TCCR1B = 0; 

    OCR1A = 7812; // 15624 - примерно 1 раз/сек. установка регистра совпадения
    TCCR1B |= (1 << WGM12); // включение в CTC режим

    // Установка битов CS10 и CS12 на коэффициент деления 1024
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);

    TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению
    sei(); // включить глобальные прерывания

//+++++++++++++++++++++++++++++++ set up PID data

  encodersGAP = 20; // Порог разницы в показаниях энкодеров. Надо переделать в encodersGapEdge
  prevT = 0;
  Eprev = 0.0;
  I = 0;
  D = 0;
  leaderIsChanged = false;

//+++++++++++++++++++++++++++++++ END of set up PID data


  Serial.begin(57600);
  Serial.println("Dual VNH5019 Motor Shield");
  md.init();
  

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(chatter);
  mobPlatformInit();

  reset_All();
//  currCommand = "getvalues"; // too fast with loop
  currCommand = "waiting";
  numRot=0;
  
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
    m1Speed = defaultMSpeed;
    m2Speed = 0.4*defaultMSpeed;
    forward(1);
  }

  if (currCommand == "left") {
    m1Speed = 0.4*defaultMSpeed;
    m2Speed = defaultMSpeed;
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
//      currCommand = "waiting";
}

if (currCommand == "slowdown") {
  decSpeed();
  currCommand = prevCommand; // Только скорость поменяли.
  }
  
if (currCommand == "faster") {
  incSpeed();
  currCommand = prevCommand; // Только скорость поменяли.
  }

//if (currCommand.startsWith("mkrotation")) { //make some rotations of any of 2 wheels
//  if (!resetDone){
//            // set prevT as current, strting time
//      prevT = micros();
//      Eprev = 0.0;
//
//      //reset_All(); // reset to zero all counters
//      posAm1 = 0;
//      posBm1 = 0;
//      posAm2 = 0;
//      posBm2 = 0;
//      m1_count = 0.0;
//      m2_count = 0.0;
//      String numOfRotations = currCommand.substring(11);
//
//      resetDone = true;
//      startTime = millis();
//
//      str = "Number of rotations is ";  str.concat(numOfRotations);
//      
//      //write2chatter(numOfRotations);
//      write2chatter(str);
//      numRot = numOfRotations.toInt();
//
//      // Start motors
//      md.setM1Speed(m1Speed);
//      md.setM2Speed(m2Speed);
//
//
//
//  }
  
  
//  if (makeRotation(numRot) == 0){  // all counters should be reset to zero in advance (before going to function makeRotations)
//     resetDone = false;
//     currCommand = "waiting"; // just output values and wait for rotations number has been reached
//
//  }
  // else currCommand = "mkrotation";
//}

//+++++++++++++++++++++++++++++++++++++
  if (currCommand.startsWith("setspeedM1")) {
    String theSpeed = currCommand.substring(11);
    m1Speed = theSpeed.toInt();
    md.setM1Speed(m1Speed);
    write2chatter("M1 speed changed");
  }

//+++++++++++++++++++++++++++++++++++++
  if (currCommand.startsWith("setspeedM2")) {
    String theSpeed = currCommand.substring(11);
    m2Speed = theSpeed.toInt();
    md.setM2Speed(m1Speed);
    write2chatter("M2 speed changed");
  }

//+++++++++++++++++++++++++++++++++++++


//  if (currCommand == "waiting") {
//      write2chatter("waiting");
//  }


          
  delay(10);

} // loop
//++++++++++++++++++++++++++++++++++++++++++++
ISR(TIMER1_COMPA_vect)
{
// Считываем значения энкодеров, выводим в serial port
// Вычисляем разницу в данных энкодеров для дальнейшего принятия решения об
// изменении скорости моторов
//  getValues(); // итак выводится в loop

// А потом будем ПИД запускать
if (currCommand.startsWith("mkrotation")){
  str = "I'm in Timer ISR function";
  write2chatter(str);
  
  if (!resetDone){
            // set prevT as current, strting time
      prevT = micros();
      Eprev = 0.0;

      //reset_All(); // reset to zero all counters
      posAm1 = 0;
      posBm1 = 0;
      posAm2 = 0;
      posBm2 = 0;
      m1_count = 0.0;
      m2_count = 0.0;
      String numOfRotations = currCommand.substring(11);

      resetDone = true;
      startTime = millis();

      str = "Number of rotations is ";  str.concat(numOfRotations);
      
      //write2chatter(numOfRotations);
      write2chatter(str);
      numRot = numOfRotations.toInt();

      // Start motors
      md.setM1Speed(m1Speed);
      md.setM2Speed(m2Speed);
  }

 
  
    if (makeRotation(numRot) == 0){  // all counters should be reset to zero in advance (before going to function makeRotations)
     resetDone = false;
     currCommand = "waiting"; // just output values and wait for rotations number has been reached

    }
    else {
      goToPID();
    } 
    
 } // if (currCommand.startsWith("mkrotation")){

  str = "Finish of Timer ISR function";
  write2chatter(str);


} // ISR(TIMER1_COMPA_vect)
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
      
//      if (strData.charAt(1)=="1"){
//           md.setM1Speed(speedVal);
//           m1Speed = speedVal;
//      }
//
//      if (strData.charAt(1)=="2"){
//           md.setM2Speed(speedVal);
//           m2Speed = speedVal;
//           stopIfFault();
//      }


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
  write2chatter(str);


  str = "posBm1 ";
  str.concat(posBm1);
  write2chatter(str);

//+++++++++++++++++++++
  str = "posAm2 ";
  str.concat(posAm2);
  write2chatter(str);


  str = "posBm2 ";
  str.concat(posBm2);
  write2chatter(str);
//+++++++++++++++++++++
  str = "intM1counter ";
  str.concat(intM1counter);
  write2chatter(str);


  str = "intM2counter ";
  str.concat(intM2counter);
  write2chatter(str);

  str = "intM2Bcounter ";
  str.concat(intM2Bcounter);
  write2chatter(str);

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
//+++++++++++++++++++++++++++++++++++++++++++++++++
// У нас срабатывают прерывания от энкодеров A1, B1, A2, B2
// Обработчик каждого из прерываний выдает значения соответствующего энкодера.
void getValues()
{
  //+++++++++++++++++++
  str = "M1_A ";
  str.concat(posAm1);
  write2chatter(str);
  
  str = "M1_B ";
  str.concat(posBm1);
  write2chatter(str);
  //+++++++++++++++++++++
  str = "M2_A ";
  str.concat(posAm2);
  write2chatter(str);
  
  str = "M2_B ";
  str.concat(posBm2);
  write2chatter(str);
  //+++++++++++++++++++++
  
  diffAbsolute = posAm1 - posAm2;
  diffRelative = diffAbsolute - diffRelative;
  str = "diffAbsolute = "; 
  str += String(diffAbsolute); str.concat(", ");
  str += "diffRealative = "; 
  str += String(diffRelative); //
  
  write2chatter(str);
  diffRelative = diffAbsolute;

  // Now pass  [diffRelative, diffAbsolute] values to correct motor speed with PID equation
    
    
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
