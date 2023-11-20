/*
 *  Программа открывает Serial port и ждет команду. 
 *  По команде "start" считывает значения энкодеров и отправляет эти данные в Serial port по таймеру 1
 *  
 *  По команде "stop" выключает отправку данных в Serial port, ждет новую команду.
 *  
 *  Используется для калибровки энкодеров. 
 * 
 *  "moveON" :
 *  - Обнуляем все счетчики таймера, запускаем его.
 *  - включаем скорость 100 на 1с. 
 *  
 *  Простой запрет прерывания таймера НЕ обнуляет его счетчики, и при следующем разрешении прерывания, т.е. запуске - он скорее всего отсчитает менее 1с.
 *  
 *  Когда сработает таймер (Через 1с) - скорость 0 (выключаем, останавливаем)
 *  
 *  "getValues" - выдаем значения энкодеров
 * 
 * ДУмаем, как прикрутить ПИД
 * 
 * 15.06.2023
 * Передаем данные в двоичном виде, так сокращаем объем, т.к. буфер serial-порта небольшой.
 * Вывод в "красивом" читаемом виде выполняем на стороне ПК
 * //++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 07.06.2023
 * Добавляем команду "blink" - будет запускать мигание диода по таймеру.
 * Важно еще выключать таймер при инициализации. Для этого надо делитель частоты устанавливать в 0, CS1[2:0]==0.
 * В нашем случае ЭТО TCCR1B=0xF8; // Timer is stopped while CS1[2:0]==0
 * А при запуске таймера TCCR1B=0x0D;
 * 
 * 
 * 26.05.2023
 * Пробую библиотеку от Алекс Гайвер
 * https://github.com/GyverLibs/GyverTimers
 * Документация https://alexgyver.ru/GyverTimers/
 * 
 * - moveON - работает так, как изначально планировалось. Но при этом продолжает давать вывод таймер.
 * 
 * - stop - Тут выключаем таймер и вывод в Serial monitor прекращается.
 * 
 * - start - Там идет разрешение прерывания, это всего одна строчка :
 *                                                            TIMSK1 |= (1 << OCIE1A); 
 *           Но после этого прога постоянно заходит в обработчик прерывания ISR (!) и процессор виснет.
 *           
 * Вот это и есть трабл. Почему сразу заходит в обработчик прерывания при установке флага OCIE1A ?           
 * Даташит, стр. 162 : "The corresponding Interrupt Vector (see “Interrupts” on page 101) is executed when the OCFnA Flag, located in TIFRn, is set."
 * 
 * OCF1A - flag in TIFR1 register
 * 
 * OCF1A: This flag is set in the timer clock cycle after the counter (TCNTn value matches the Output Compare Register A(OCRnA).
 * 
 * Проблему с входом в прерывание можно пока игнорировать
 * По прерыванию мы только делаем вызов ф-ции, где считываем значения энкодеров (и отправляем их по Serial Port в ПИД), а далее запускаем процедуру вычисления ПИД,
 * С последующей отправкой новых значений ПИД-коэффициентов.
 * 
 * 
 * 
 * 
 * //+++++++++++++++++++++++++++++++++++++++++++++++++
 * 24.05.2023
 * Добавляем команду "p1", "p2", где 1 или 2 - это номер колеса.
 * При получении этой команды увеличиваем скорость соответствующего колеса на 10%
 * Следим за изменением отставания колеса.
 * На сегодня М1 стабильно отстает - прям исходное состояние.
 * 
 * Режим таймера непонятен. Как только он запускается - сразу входит в обработчик прерывания, либо уже имеет наполненное значение...
 * Но... даже если сразу входит в обработчик, что скорость моторов не обнуляет, хотя код там есть.
 * 
 * 
 */

#include <util/atomic.h>
#include <avr/interrupt.h>
//#include <GyverTimers.h>


#include <stdlib.h>
#include "DualVNH5019MotorShield.h"
//#include <std_msgs/String.h>

struct Enc {
  word A1_Enc;         // 2 bytes
  word A2_Enc;         // 2 bytes
  short diff;          // 2 bytes
  short diff_vector;   // 2 bytes
  int M1_Speed;        // 2 bytes
  int M2_Speed;        // 2 bytes
  unsigned long time_ms; // 32 bits current time in ms
  float deltaT;          // 32 bits
  float dedt;            // 32 bits
  //++++++++++++ 24 bytes above +++++++++++++++++
  int E;                // 2 bytes
  int Eprev;            // 2 bytes
  int lagSpeed_ptr;          // 2 bytes, Значение после ПИД
  int fwdSpeed_ptr;         // 2 bytes, Значение после ПИД
  int M1Speed_start;          // 2 bytes
  int M2Speed_start;          // 2 bytes
  //++++++++++++ 36 bytes above
  float Integral;           // 4 bytes
  char mystatus[8];         // 8 bytes string interpreting current device status
  unsigned long timestamp;  //4 bytes
  //++++++++++++ 52 bytes above
  float Proportional;           // 4 bytes
  float Integral_k;             // 4 bytes
  float Derivative;             // 4 bytes

  char mytext[43];              // 43 bytes memo for debugging
  
}; //107 bytes total

// running
// stopped

volatile Enc data;

String commandList[10] = {"blink","start", "stop", "moveon", "setPID", "reset", "getValues" ,"samplingTime", "calibration", "setSpeedInit"};

DualVNH5019MotorShield md; // Motor Driver

#define sBufSize 32   // Размер буфера компорта в плате NANO - 64 байта.
#define szParcel 8    // Размер посылки по Serial port

char buf[sBufSize];

byte ints[sBufSize]; // Данные, полученные по serial


String str="";
volatile String currCommand = ""; // Текущая команда

const  byte pinAm1 = 19; // Пины драйвера Polulu
const  byte pinBm1 = 18; // 

const byte movingThreshold = 75; //20;



//++++++++++++++++++++++++ Начало вращения колеса в режиме "на вису"
int m1LightSpeed = 5;//27;
int m2LightSpeed = 5; //24;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int defaultMSpeed = 25;
int cruiseMSpeed = 50/2;
int speedBottomLimit = round(0.7*min(m1LightSpeed, m2LightSpeed));// 28; 
int speedTopLimit = round(2.1*min(m1LightSpeed, m2LightSpeed));//150;

volatile int m1Speed = defaultMSpeed; // Левое колесо
volatile int m2Speed = defaultMSpeed; // Правое колесо

//++++++++++++++++++++++++ Начало вращения колеса в режиме "на земле"
volatile int startM1Speed = m1LightSpeed;// + 50; //92;//102; //121; //82;  // Порог старта движения для М1
volatile int startM2Speed = m2LightSpeed;// + 50; //89; //108; //67;  // ... для М2 


//volatile int *fwdmSpeed; // Скорость опережающего
//volatile int *lagmSpeed; // Скорость отстающего колеса


volatile long posAm1 = 0;
volatile long posBm1 = 0;

volatile int diameter; // 90mm
volatile int m1A_count, m1B_Count, m2A_count, m2B_count;
volatile double m1_count, m2_count;
volatile int m1A_k, m1B_k, m2A_k, m2B_k; // число отсчетов энкодера на 1 оборот колеса. Здесь k - коэффициент

// Возможно это дубль. Оставляем, чтобы компилятор проходил.
volatile long diffAbsolute = 0; // Абсолютная разница в энкодерах 1-го и 2го колеса 
volatile long diffRelative = 0; // Относительная разница(относительно предыдущих показаний) в энкодерах 1-го и 2-го колеса.

//+++++++++++++++++++++++++++++++++++++++++ PID variables +++++++++++++++++++
// volatile double Kp, Ki, Kd;
// volatile float P, I, D;
float Kp = 0.0; // 0.0235;
float Ki = 0.0; // 0.01;
float Kd = 0.0; // 0.0275;
 

const int timerPerRotation = 2; // Сколько раз срабатывает таймер за 1 оборот колеса
volatile double TRC = (double)1/timerPerRotation; // timer/rotation coefficient == dt т.к. таймер за 1 сек.


boolean isSomeOneLeading; // Флаг смены отстающего колеса на ведущее. Если М1_А > M2_A isSomeOneLeading == false, т.к. обычно М1 крутится быстрее М2
                         // Если отстающее колесо обгоняет ведущее, то значит меняем их статусы на противоположные.
                         // При этом скорость колес будет нарастать.
                         // Значит скорость сбрасываем, когда разница в показаниях меньше 


volatile long prevT = 0;    // for delta t counting
volatile int Eprev = 0;     // for delta Error counting
volatile int dltEprev = 0; 

volatile int E;

volatile long myprevT = 0;

unsigned long msPrevT = 0; // Для фиксации интервалов проверки E
unsigned long msCurrT = 0;

unsigned long movingTime = 0;
unsigned long elapsedTime = 0;
unsigned long lastPidTime  = 0; // Фиксируем время последнего срабатывания PID
unsigned long isItPidTime = 0;  // А не пора ли запускать ПИД ?

volatile unsigned long measureTime = 6000; // Время движения в мс
volatile unsigned int pidTime = 200; // Интервал в мс между корректировками скорости.

volatile float Eintegral = 0;

volatile byte encodersGAP = 50;                               // То включаем ПИД

volatile bool pid1stTime = true; // Флаг запуска ПИД. самый первый раз, после запуска loop    

long Timer1A_OneSec = 15624;

float t1 = 0.521; // test
float t2 = 15.125;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

volatile long posAm1_prev = 0; // previous value of Am1
volatile long posAm2_prev = 0; // previous value of Am2

volatile long TimerA1_prev = 0; // previous value of A1 in timer
volatile long TimerA2_prev = 0; // previous value of A2 in timer


volatile int diff = 0; // difference between posAm1, posAm2
volatile int diff_prev = 0; // previous difference between posAm1, posAm2


const  byte pinAm2 = 20;  //(PE4 for ATMega2560, Digital 2-pin)
const  byte pinBm2 = 21;  //(PE5 for ATMega2560, Digital 3-pin)
volatile long posAm2 = 0;
volatile long posBm2 = 0;

const int M1 = 480; // Мотор М1 Число отсчетов энкодера на 1 оборот колеса
const int M2 = 482; // Мотор М2 Число отсчетов энкодера на 1 оборот колеса

volatile long stopCounter = 0;

//volatile long intM1counter = 0;
//volatile long intM2Bcounter = 0;

volatile int pinAm1Value = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int pinBm1Value = 0;   // Переменные хранящие состояние пина, для экономии времени

volatile int pinAm2Value = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int pinBm2Value = 0;   // Переменные хранящие состояние пина, для экономии времени

volatile bool startFlag = false; // Началось движение тележки


const unsigned long ONE_SECOND = 15624;
const unsigned long TWO_SECOND = 31248;

unsigned long ocrValue = 7812; // 46872 == 3c. 31248==2c 15624==1c 7812==0.5c 3906==0.25c 1953 = 0.125c 976 = 60ms

unsigned long myTime;

int zeroSpeed = 0;

void parse_command();  // Обработка данных от Serial Port
void reset_All();      // Обнуление счетчиков энкодеров
void Am1();            // Обработчик прерывания от энкодера А1
void Bm1();            // Обработчик прерывания от энкодера B1
void Am2();            // Обработчик прерывания от энкодера А2
void Bm2();            // Обработчик прерывания от энкодера B2

void pid();
void MoveIfStopped();
void goToPID();
void struct_init();

void decreaseSpeed();

int dtmsplit(char *str, const char *delim, char ***array, int *length); 

void updateLimits(); //  Меняем значения верхнего и нижнего предела скоростей.

#define SPTR_SIZE   20
char   *sPtr [SPTR_SIZE];
char   *strData = NULL;

//int EK_separate (
//      String str,
//      char   **p,
//      int    str_size );

int separate (String& str,  // pass as a reference to avoid double memory usage
              char **p,  
              int size, 
              char **pdata );

void freeData(char **pdata);

void findStartCurrent(); //  определние скорости "отрыва от земли" - сила тока, при которой каждое из колес начинает вращаться в положении "на вису".

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setup()
{
 // pinMode(LED_BUILTIN, OUTPUT);

  pinMode(pinAm1, INPUT);           // Пины в режим приема INPUT
 // pinMode(pinBm1, INPUT);           // Пины в режим приема INPUT

  pinMode(pinAm2, INPUT);           // Пины в режим приема INPUT
 // pinMode(pinBm2, INPUT);           // Пины в режим приема INPUT


  attachInterrupt(digitalPinToInterrupt(pinAm1), Am1, RISING);  // Настраиваем обработчик прерываний по изменению сигнала CHANGE
//  attachInterrupt(digitalPinToInterrupt(pinBm1), Bm1, RISING);  // Настраиваем обработчик прерываний по изменению сигнала

  attachInterrupt(digitalPinToInterrupt(pinAm2), Am2, RISING);  // Настраиваем обработчик прерываний по изменению сигнала CHANGE
//  attachInterrupt(digitalPinToInterrupt(pinBm2), Bm2, RISING);  // Настраиваем обработчик прерываний по изменению сигнала

//+++++++++++++++ set up timer 1
    // инициализация Timer1
    // BYTE &= ~(1 << BIT); - установка бита "BIT" в 0
    // 16 000 000 / 1024 = 15625 (ГЦ - частота тактов счетчика Т1), соответственно за 1с у нас 15625 импульсов, пишем это число в регистр OCR1A
    // (target time) = (timer resolution) * (# timer counts + 1)

    cli(); // отключить глобальные прерывания
    TCCR1A = 0; // установить регистры в 0
//    TCCR1B = 0; // Timer/Counter остановлен

// In CTC mode the counter is cleared to zero when the counter value (TCNTn) matches either the OCRnA (WGMn3:0 = 4)...
// {WGM11, WGM10} э TCCR1A
// {WGM13, WGM12} э TCCR1B

// WGM13     WGM12    WGM11   WGM10
//   0         1        0       0

    //TCCR1B = 0x0D;
    
//      7       6     5     4       3       2       1       0
//    ICNC1   ICES1   -   WGM13   WGM12   CS12    CS11    CS10 

    TCCR1B=0x08; // Timer is stopped while CS1[2:0]==0
//
//
//
    OCR1A = ocrValue ; //ocrValue 15624 - примерно 1 раз/сек. 7812 - примерно 2раза/с. установка регистра совпадения
//    TCCR1B |= (1 << WGM12); // включение в CTC режим - "Clear Timer in Compare Match - CTC Mode"
//                            // WGM12 - бит WGM2 для таймера 1 ставим в "1" - режим 4(СТС)

    // Установка битов CS10 и CS12 на коэффициент деления 1024
//    TCCR1B |= (1 << CS10);
//    TCCR1B |= (1 << CS12); // ==0x05
      

//    TIMSK1 = (0 << OCIE1A); // установка бита "OCIE1AT" в 0 - выключаем таймер
    TCNT1 = 0; // Обнуляем счетчик
    TIMSK1 &= ~(1 << OCIE1A);  // Выключаем бит. включение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
    TIMSK1 = 0x00;
    sei(); // включить глобальные прерывания

// На вису
//  m1Speed=28;
//  m2Speed=26; 

// На земле
//  m1Speed=82;
//  m2Speed=67; 
  
  Serial.begin(115200); // Serial Speed, default baud rate for rosserial_python
  Serial.println("Waiting for PID values"); //Dual VNH5019 Motor Shield
  Serial.flush(); // It is only relevant when the Arduino is sending data and its purpose is to block the Arduino until all outgoing the data has been sent.
  md.init();
 // delay(100);

  // mobPlatformInit();

  reset_All();
  currCommand = "stop";
  struct_init(); // Инициализация структуры, хранящей посылку в ПК


 
}
//++++++++++++++++++++++++++++++++++++++++++++++++
 
 
void loop()
{

 parse_command();
//+++++++++++++++++++++++++++++++++++++++++++++  Просто едем вперед с постоянной скоростью 
   if (currCommand == "starting"){
     //  startNoTimer();
       movingTime = millis();    // Фиксируем время начала движения
       lastPidTime = movingTime; // Фиксируем время последнего срабатывания PID

       //currCommand = "moving";
       
       startFlag = true;

    }

 
//+++++++++++++++++++++++++++++++++++++++++++++  Едем на время, включаем пид 

 if (currCommand == "moving" || currCommand == "pid" || currCommand == "calibration"){// || currCommand == "calibration"){// Считаем время движения, если истекло - останавливаем.
  msCurrT = millis();
  elapsedTime = msCurrT - movingTime;
  
//  str = "ElapsedTime= ";
//  str += String(elapsedTime); str += ", ";
//  str += "movingTime= ";
//  str += String(movingTime); str += ", ";
//  
//    Serial.println(str);
//    Serial.flush();


  if (msCurrT >= lastPidTime + pidTime){ // Is it time to start PID ?
    int diff = posAm1 - posAm2 ;
    int delta = 0;
    
    lastPidTime = msCurrT;
   
     
    if (diff<0){delta= -1*diff;}
    else {
      delta = diff;
      }
            
    //if (delta >= encodersGAP && currCommand == "pid"){ 
    if (currCommand == "pid"){ 
    
            //currCommand == "pid")
            pid();
            pid1stTime = false;
           
      }
    if (currCommand == "moving"){ // значит moving
//      if (currCommand == "pid") { // Т.е. в процессе движения в режиме PID разницы почти нет, тогда может в обратную сторону уже крутить ?
        // Тогда тут нужна какая-то особая ф-ция, или параметр для ф-ции pid()
        //currCommand = "moving";
        // Eprev = E;
        data.A1_Enc = posAm1;
        data.A2_Enc = posAm2;
        data.diff = diff;
        data.Eprev = Eprev;

        str = "!!! ABS(diff)="; str += String(delta); str += " is less than GAP="; str += String(encodersGAP); str += " !!!";
        str.toCharArray(data.mytext, sizeof(data.mytext));
        data.time_ms = millis();

        Serial.write((byte*)&data, sizeof(data));
        Serial.flush();
        E = diff;
        Eprev = diff;
        prevT = msCurrT;
         
//        }//if
      } // if moving

      
      
    } //  (msCurrT >= lastPidTime + pidTime)
    
  if (elapsedTime >= measureTime){ // Истекло, встаем
    stopPlatform();
    currCommand = "finish--";

     Eprev = 0;

    } //(elapsedTime >= measureTime)
  
  } // (currCommand == "moving";)

  if (currCommand == "finish--"){ // Проверяем полную остановку
    if (posAm1 == posAm1_prev && posAm2 == posAm2_prev){

      currCommand = "stopPOSQ";
      data.A1_Enc = posAm1;
      data.A2_Enc = posAm2;
      E = posAm1 - posAm2;
      data.diff = E;
      data.E = E;
      data.Integral = 333.0;
      data.time_ms = millis();
      currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));

      str = "  currCommand == finish--  and stopped NOW "; 
      str.toCharArray(data.mytext, sizeof(data.mytext));


      
      Serial.write((byte*)&data, sizeof(data));
      Serial.flush();
      reset_All();
      
      }
    else {
              delay(200);
//            posAm1_prev = posAm1;
//            posAm2_prev = posAm2;
      data.A1_Enc = posAm1;
      data.A2_Enc = posAm2;
      E = posAm1 - posAm2;
      data.diff = E;
      data.E = E;
      data.Integral = 999.0;
      data.time_ms = millis();
      currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));
      
      str = "  Not stopped yet, finishing   "; 
      str.toCharArray(data.mytext, sizeof(data.mytext));

      
      Serial.write((byte*)&data, sizeof(data));
      Serial.flush();


      }
    }
 
   
delay(10);
    posAm1_prev = posAm1;
    posAm2_prev = posAm2;

} // loop
//+++++++++++++++++++++++++++++++++++++++++++++++++
// У нас срабатывают прерывания от энкодеров A1, B1, A2, B2
// Обработчик каждого из прерываний выдает значения соответствующего энкодера.
void getValues(float &delta_T, float &dedt)
{
  unsigned long currentMillis;
  //+++++++++++++++++++
//  str = "M1_A ";
//  str.concat(posAm1);
//  Serial.println(str);
//  Serial.flush();
//
//  
//  str = "M1_B ";
//  str.concat(posBm1);
//  Serial.println(str);
//  Serial.flush();
//
//  //+++++++++++++++++++++
//  str = "M2_A ";
//  str.concat(posAm2);
//  Serial.println(str);
//  Serial.flush();
//  
//  str = "M2_B ";
//  str.concat(posBm2);
//  Serial.println(str);
//  Serial.flush();
  //+++++++++++++++++++++

//  data.A1_Enc = 936; //posAm1;
//  data.A2_Enc = 743; //posAm2;
//  diff_prev = 500; //diff;
  
  //diff_prev = posAm1_prev - posAm2_prev;



  diff = posAm1 - posAm2;

  data.diff = diff;
  data.diff_vector = diff-diff_prev;


         
          
    str = "TIME = "; 
    str += String(millis());
    str += ", ";
  
  str += "posAm1=";
  str += String(posAm1); str += ", ";
  str += "posAm2=" ;
  str += String(posAm2); str += ", ";
  
  str += "diff="; 
  str += String(diff); str += ", ";
  
  str += "diff_prev=";
  str += String(diff_prev); str += ", ";

  str += "VectorDiff=";
  str += String(abs(diff-diff_prev)); str += ", ";

 
  
  str += "m1Speed=";
  str += String(m1Speed);  str += ", ";
  
  str += "m2Speed=";
  str += String(m2Speed); str += ", ";

  // Однако, время заложено в deltaT
//  str += "millis= ";
//  str += String(millis());
//  str += '\n';
  
//  data.A1_Enc = posAm1;
//  data.A2_Enc = posAm2;
//
//  data.M1_Speed = m1Speed;
//  data.M2_Speed = m2Speed;
//  data.time_ms = millis();
//  data.deltaT = delta_T; // 1234.5678; 
//  data.dedt = dedt; //3.5689;  
//  data.E = posAm1 - posAm2;
//  data.Eprev = Eprev;
//  
//  data.lagSpeed_ptr = &m1Speed;
//  data.fwdSpeed_ptr = &m2Speed;
//  data.M1Speed_ptr = &m1Speed;
//  data.M2Speed_ptr = &m2Speed;
//
//  data.Integral = 0.0;
  
  //Serial.write((byte*)&data, sizeof(data));
  Serial.println(str);
  Serial.flush();
  diff_prev = diff;
 
  
/*  diffAbsolute = posAm1 - posAm2;
  diffRelative = diffAbsolute - diffRelative;
  str = "diffAbsolute = "; 
  str += String(diffAbsolute); str.concat(", ");
  str += "diffRealative = "; 
  str += String(diffRelative); //
  
  write2chatter(str);
  diffRelative = diffAbsolute;
*/
  // Now pass  [diffRelative, diffAbsolute] values to correct motor speed with PID equation
    
    
} // getValues
//+++++++++++++++++++++++++++++++++++++
// Запускаем движение тележки в режиме работы с таймером.
void startPlatform(){

  reset_All();
  int posA1;
  int posA2;

  currCommand = "starting";
  // 24 & 27
  m1Speed=round(0.75 * m1LightSpeed);// + 30; startM1Speed - 32; // m1LightSpeed + K1
  m2Speed=round(0.75 * m2LightSpeed);// + 25; startM2Speed - 40; // m2LightSpeed + K2

       
  md.setM1Speed(m1Speed); //defaultMSpeed
  md.setM2Speed(m2Speed);
  delay(200);
  
  float f1Speed = 0.0;
  float f2Speed = 0.0; // вещественное значение Скорости
  posA1 = posAm1;
  posA2 = posAm2;

//  str = "posAm1="; 
//  str += String(posAm1); str += ", ";
//  str += "posAm2=" ; 
//  str += String(posAm2); str += ", ";
//  Serial.println(str);
//  Serial.flush();
  bool m1Started = false;
  bool m2Started = false;

 
  //while ((posA1 < 200) || (posA2 < 200))
   while (!m1Started || !m2Started) 
  {
    delay(5);
   // Если не сдвинулся, то увеличиваем скорость
  if (!m1Started) {f1Speed += 0.2; m1Speed += round(f1Speed); md.setM1Speed(m1Speed);}// 1.1*m1Speed;
  if (!m2Started) {f2Speed += 0.2; m2Speed += round(f2Speed); md.setM2Speed(m2Speed);}// 1.1*m2Speed;
           
    delay(200);
    if (posAm1 > 200) {m1Started = true; startM1Speed = m1Speed;}; //  str = "M1 Started at " ; str += String(m1Speed); Serial.println(str);}
    if (posAm2 > 200) {m2Started = true; startM2Speed = m2Speed;}; //  str = "M2 Started at " ; str += String(m2Speed); Serial.println(str);}
    posA1 = posAm1;
    posA2 = posAm2;
   
  }
//    posAm1_prev = posA1;
//    posAm2_prev = posA2;
 
//  delay(250);
 
  // Мы поехали, но скорость  M1 > startM1Speed и M2 > startM1Speed
  // А теперь снижаем до порога начала движения.
  // Учитываем, что этот порог у колес разный.
      
  // Скорости, на которых тронулись
//  str = "m1Speed="; 
//  str += String(m1Speed);  str += ", ";
//  
//  str += "m2Speed="; 
//  str += String(m2Speed);
//  str += ", ";
//  str += "posAm1="; 
//  str += String(posAm1); str += ", ";
//  str += "posAm2=" ; 
//  str += String(posAm2); str += ", ";
//
// str += "\r\n";
        
        
//  Serial.println(str);
//  Serial.flush();

//++++++++++++++++++++++++++++++ Актуально только в режиме "Floor"

//   bool finished = false;
//   while (!finished){
//      
//      // (+12/+5) - это запас на преодоление неровностей. Для разных пов-тей будет свой. И "умный" робот должен его сам менять.
//      if (m1Speed > startM1Speed +10) {m1Speed -= 1; md.setM1Speed(m1Speed);}
//      if (m2Speed > startM2Speed +5) {m2Speed -= 1; md.setM2Speed(m2Speed);}
//  
//      if ((m1Speed <= startM1Speed + 10) && (m2Speed <= startM2Speed +5)) {finished = true;}
//      delay(200);
//      posAm1_prev = posAm1;
//      posAm2_prev = posAm2;
//  
//  
//    }
////       m1Speed=82; md.setM1Speed(m1Speed);
////       m2Speed=67; md.setM2Speed(m2Speed);
//       
//  md.setM1Speed(m1Speed);
//  md.setM2Speed(m2Speed);
  
//++++++++++++++++++++++++++++++ End of Актуально только в режиме "Floor"
  
//  str = "Updated m1Speed="; 
//  str += String(m1Speed);  str += ", ";
//  
//  str += "Updated m2Speed="; 
//  str += String(m2Speed); 
//  str += ", ";
//  str += "posAm1="; 
//  str += String(posAm1); str += ", ";
//  str += "posAm2=" ; 
//  str += String(posAm2); str += ", ";

//        str += "\r\n";
//        Serial.println(str);
//        Serial.flush();
        
//        diff_prev = abs(posAm1 - posAm2);
//        posAm1_prev = posAm1;
//        posAm2_prev = posAm2;

        
//        delay(250);

        currCommand = "moving"; 


        // Девайс тронулся, включаем таймер
        // Запускаем таймер
        
//        cli();  // отключить глобальные прерывания
//        OCR1A = ocrValue ; 
//        TCNT1 = 0;
//        TCCR1B=0x0D;
//        TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
//        sei(); // включить глобальные прерывания
 
  } // startPlatform()
//+++++++++++++++++++++++++++++++++++++
// Обычный старт, но без таймера
void startNoTimer(){
  int posA1;
  int posA2;

  md.setM1Speed(speedBottomLimit);
  md.setM2Speed(speedBottomLimit);
  delay(70);

  float f1Speed = 0.0;
  float f2Speed = 0.0; // вещественное значение Скорости
  posA1 = posAm1;
  posA2 = posAm2;
  while (((posA1 < 70) || (posA2 < 70)) || ((m1Speed < cruiseMSpeed) || (m2Speed < cruiseMSpeed)) )
  {
    delay(5);
   // Если не сдвинулся, то увеличиваем скорость
    f1Speed += 0.1; // 1.1*m1Speed;
    f2Speed += 0.1; // 1.1*m2Speed;
  
    m1Speed = speedBottomLimit +  round(f1Speed);
    m2Speed = speedBottomLimit +  round(f2Speed);
    
    md.setM1Speed(m1Speed);
    md.setM2Speed(m2Speed);
    delay(40);
    posA1 = posAm1;
    posA2 = posAm2;
  
  }
  delay(1000);

  // Стартанули, теперь понижаем скорость, до значения вблизи порога остановки.
  while (m1Speed > movingThreshold || m2Speed > movingThreshold){
    m1Speed -=1;
    if (m1Speed < movingThreshold) {m1Speed = movingThreshold;}

    m2Speed -=1;
    if (m2Speed < movingThreshold) {m2Speed = movingThreshold;}

    md.setM1Speed(m1Speed);
    md.setM2Speed(m2Speed);
    delay(100);
    
  } // while threshold
  
  
  
//  startM1Speed = m1Speed;
//  startM2Speed = m2Speed;
  



  } //  startNoTimer()
//+++++++++++++++++++++++++++++++++++++
void stopPlatform(){
        currCommand = "stopped!";
        startFlag==false; // Готовы к старту.
        pid1stTime = false;  // Останавливаем ПИД
        //reset_All();
        TIMSK1 &= ~(1 << OCIE1A);  // выключение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
        md.setM1Speed(0);
        md.setM2Speed(0);
//        Serial.println("Stopped");
//        Serial.flush();

  } //stopPlatform()
//+++++++++++++++++++++++++++++++++++++
void parse_command()
{
  String message = "";

  if (Serial.available()) {
    message = Serial.readString();
    message.trim();
    Serial.flush();

  
      if (message == "enc1pivot"){ // Определяем число отсчетов за 1 цикл работы таймера
          
           Serial.println("It's pivot tuning");
        } //"enc1pivot"
      
      
      if (message == "blink") // Разрешаем прерывания таймера 1
      {
        ocrValue = ONE_SECOND;
        cli();  // отключить глобальные прерывания
        OCR1A = ocrValue ; 
        TCNT1 = 0;
        TCCR1B=0x0D;
        TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
        sei(); // включить глобальные прерывания
  
       myTime=millis();
       str = "blink start time ";
       str += String(myTime);
       Serial.println(str);
       Serial.flush();
        
      }// "blink"

      if (message == "start") // Разрешаем прерывания таймера 1
      {
        message = "";
        currCommand = "start";
        reset_All();

        startPlatform();
        // Сразу переходим в pid
        currCommand = "pid";  // Переходим сразу в режим pid
        movingTime = millis();    // Фиксируем время начала движения
        prevT = movingTime;     // Чтобы ПИД
        Eintegral = 0;            // Чтобы ПИД
        E = posAm1 - posAm2;
        Eprev = 0;
        lastPidTime = movingTime; // Фиксируем время последнего срабатывания PID, т.е. только что

        data.A1_Enc = posAm1;
        data.A2_Enc = posAm2;

        data.diff = E;
        
        data.M1_Speed = m1LightSpeed;//m1Speed;
        data.M2_Speed = m2LightSpeed;//m2Speed;
        data.time_ms = movingTime;
        data.deltaT = 0.2; // сохраняется как 0.004999999888241291
        data.dedt = 0.0; //3.5689;  
        data.E = E;
        data.Eprev = Eprev; // Потом при срабатывании PID это используем
        
        data.lagSpeed_ptr = 0;//m1Speed; // Только стартанули, еще нет отстающих/опережающих
        data.fwdSpeed_ptr = 0;//m2Speed;
        data.M1Speed_start = &m1Speed;
        data.M2Speed_start = &m2Speed;
        
        data.Integral = 777.0;

        str = "started!";
        str.toCharArray(data.mystatus, sizeof(data.mystatus));
        data.timestamp = movingTime;
        data.Proportional = Kp;
        data.Derivative = Kd;
        str = "Just Started with minimum speed values";
        str.toCharArray(data.mytext, sizeof(data.mytext));

    Serial.write((byte*)&data, sizeof(data));
    Serial.flush();
    Eprev = E;
    
      } //if (message == "start")
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (message == "stop") // Запрещаем прерывания таймера 1 - перестаем слать данные в Serial Port
      {
        currCommand = "stop";
        startFlag==false; // Готовы к старту.
        pid1stTime = false;  // Останавливаем ПИД
        //reset_All();
        TIMSK1 &= ~(1 << OCIE1A);  // включение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
        md.setM1Speed(0);
        md.setM2Speed(0);
//        Serial.println("Stopped");
//        Serial.flush();
        }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++ Parameters ++++++++++++++++++++++++++++++++
    if (message.startsWith("setPID")){ // Задаем PID-коэффициенты, число циклов запуска платформы, длительность в мс каждого запуска.
       // setPID 0.0256 2.3578 3.8754 7 5500 400
       //message = "setPID 0.0235 222.2222 3333.3333 7 5500 400";
    str = message.substring(7);
    
      String K;
      int Num;
      String mystr;

      currCommand = "waiting";
      //int N = EK_split_str(str, sPtr, SPTR_SIZE);      
      int N = separate (str, sPtr, SPTR_SIZE,  &strData);

      //внутри цикла данные еще правильные :) после него что-то ломается в массива sPtr. Поэтому цикл не используем.
//         for (int n = 0; n < N; n++){
//            K = sPtr [n];
//              mystr = "K = ";  mystr += K; //String(Kx,4); mystr += " ";
//              Serial.println (mystr); //sPtr [n]
//              Serial.flush();
//              }// for
       


      K = sPtr[0];
      Kp = K.toFloat()*10000;
      //float Kpp = //float(235/10000);  1000 * 0.0235; 
      //float Kpp = 235/1000;

      str = "Kp= "; 
      str += String(Kp);
      str += " ## ";
      
 //freeData(&strData);
     
      K = sPtr [1];
      Ki = K.toFloat()*10000;
      str += "Ki= ";
      str += String(Ki);
      str += " ## ";
      
      K = sPtr [2];
      Kd = K.toFloat()*10000;

      str += "Kd= ";
      str += String(Kd);
//      Serial.println(str);
//      Serial.flush();    

      
//      Kd /= 10000;
      K = sPtr[3];
      Num = K.toInt();  // Число циклов запуска платформы
//      str = "N = "; 
//      str += String(Num);
//      Serial.println (str); 
//
      K = sPtr[4];
      Num = K.toInt();
      measureTime = Num; // Время работы в мс 1 цикла запуска платформы.
      str += "Tfull = "; 
      str += String(Num);
      str.toCharArray(data.mytext, sizeof(data.mytext));

//
      K = sPtr[5];
      Num = K.toInt();
      pidTime = Num;    // Время движения с постояннмыи ПИД-настройками, 400 - это время действия ПИД в мс.
      //pidTime = 400;
      
     str = "Tpid = "; 
     str += String(pidTime);
     str += ", ";
     str += "N = ";
     str += String(N);
     str += ", ";
     str += "sPtr[5] = ";
     str += sPtr[5]; 
//     Serial.println(str);
//     Serial.flush();    

//      Kp = 12.3456;
//      Kd = 34.3456;
//      
      data.timestamp = millis();
      str = "Got-PID!";
      str.toCharArray(data.mystatus, sizeof(data.mystatus));

      data.Proportional = Kp;
      data.Integral_k = Ki;
      data.Derivative = Kd;

     Serial.write((byte*)&data, sizeof(data));
     Serial.flush();

  } // setPID
    

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

 if (message.startsWith("samplingTime")){ // Задаем Время длительности 1 цикла движения в секундах
    //samplingTime 10
    str = message.substring(sizeof("samplingTime"));
    unsigned int sTime;
    sTime = str.toInt();

    Serial.println(String(sTime));
    Serial.flush();

  
  
 }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 if (message == "getTimes"){ // Вывести текущие значения measureTime pidTime
  str = "measureTime = ";
  str += String(measureTime); str += ", ";
  str += "pidTime = ";
  str += String(pidTime);  str += ", ";

  str.toCharArray(data.mytext, sizeof(data.mytext));

//  Serial.write((byte*)&data, sizeof(data));
//  Serial.flush();

  
  
  Serial.println(str);
  Serial.flush();

        
      }

 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
      if (message == "moveon") // Запускаем тележку на 5с., останавливаем, передаем показатели в ПК
      {
        
       reset_All();
       // Перед запуском двигателей сохраняем текущие значения как предыдущие
      
       //startNoTimer();
       startPlatform();
       
       movingTime = millis();    // Фиксируем время начала движения
       lastPidTime = movingTime; // Фиксируем время последнего срабатывания PID

       currCommand = "moving";
       
       startFlag = true;
       diff = posAm1 - posAm2;

        data.A1_Enc = posAm1;
        data.A2_Enc = posAm2;
        data.diff = diff;
        
        data.M1_Speed = m1Speed;
        data.M2_Speed = m2Speed;
        data.time_ms = movingTime;
        data.deltaT = 0.005; // 1234.5678; 
        data.dedt = 0.0; //3.5689;  
        data.E = diff;
        data.Eprev = 0; // Потом при срабатывании PID это используем
        
        data.lagSpeed_ptr  = m1Speed;
        data.fwdSpeed_ptr  = m2Speed;
        data.M1Speed_start = &m1Speed;
        data.M2Speed_start = &m2Speed;
        
        data.Integral = 777.0;
        
        currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));
        data.timestamp = movingTime;
        data.Proportional = Kp;
        data.Derivative = Kd;

        str = "moveon started, pid launching as optional";
        str.toCharArray(data.mytext, sizeof(data.mytext));


    Serial.write((byte*)&data, sizeof(data));
    Serial.flush();

//  Serial.println("Start Moving !!!" + String(movingTime));
       
     } //"moveON"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (message == "reset") // Разрешаем прерывания таймера 1
      {
        reset_All();
          Serial.println("Done");
          Serial.flush();

 
        }
//++++++++++++++++++++++++++++++++++
      if (message == "getValues") // Просто выводим текущие данные энкодеров
      {
          diff = abs(posAm1 - posAm2);
          
          str = "TIME = "; 
          str += String(millis());
          str += ", ";
          str += "posAm1="; 
          str += String(posAm1); str += ", ";
          str += "posAm2=" ; 
          str += String(posAm2); str += ", ";
          
          str += "diff="; 
          str += String(diff); str += ", ";

          str += "diff_prev=";
          str += String(diff_prev); str += ", ";

          str += "ocrValue="; 
          str += String(ocrValue);  str += ", ";

          str += "m1Speed="; 
          str += String(m1Speed);  str += ", ";

          str += "m2Speed="; 
          str += String(m2Speed); str += "\r\n";

          
          Serial.println(str);
          Serial.flush();

        
      } //"getValues"

      if (message == "setspeed"){
        md.setM1Speed(m1Speed); // speedBottomLimit
        md.setM2Speed(m2Speed);

//        m1Speed = speedBottomLimit;
//        m2Speed = speedBottomLimit;

        }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (message == "m+"){

          posAm1_prev = posAm1;
          posAm2_prev = posAm2;

          while ((posAm1 - posAm1_prev) < 50){

            m1Speed += 1;
            m2Speed += 1;
            md.setM1Speed(m1Speed);
            md.setM2Speed(m2Speed);

            delay(400);
            
          }

          str = "M++ speed = ";
          str += String(m1Speed);

          Serial.println(str);
          Serial.flush();
          currCommand = "moving";
          
          bool finished = false;
          while (!finished){
          m1Speed -= 1;
          m2Speed -= 1;
          if (m1Speed > startM1Speed) {md.setM1Speed(m1Speed);}
          if (m2Speed > startM2Speed) {md.setM2Speed(m2Speed);}

          if ((m1Speed <= startM1Speed) && (m2Speed <= startM2Speed)) {finished = true;}
          delay(250);

        }


          

        }
//++++++++++++++++++++++++++++++++++        

        if (message == "m-"){
          m1Speed -= 1;
          m2Speed -= 1;
          md.setM1Speed(m1Speed);
          md.setM2Speed(m2Speed);

          str = "speed = ";
          str += String(m1Speed);

          Serial.println(str);

        }
//+++++++++++++++++++++++++++++++++++

      if (message == "decrease"){
        decreaseSpeed();
        
        }
     
//++++++++++++++++++++++++++++++++++++++++++++++

    if (message == "A1+"){
      m1Speed +=1;
      md.setM1Speed(m1Speed);
      }
//++++++++++++++++++++++++++++++++++++++++++++++
    if (message == "A2+"){
      m2Speed +=1;
      md.setM2Speed(m2Speed);
      }
//++++++++++++++++++++++++++++++++++++++++++++++
      if (message == "A1-"){
      m1Speed -=1;
      md.setM1Speed(m1Speed);
      }
//++++++++++++++++++++++++++++++++++++++++++++++
    if (message == "A2-"){
      m2Speed -=1;
      md.setM2Speed(m2Speed);
      }
//++++++++++++++++++++++++++++++++++++++++++++++
    if (message == "calibration"){
      currCommand = "calibration"; // Рановато ? уже сейчас пошел отсчет времени elapsedTime ? [нет, не сейчас.]
      findStartCurrent();
      // Вот мы поехали.
      currCommand.toCharArray(data.mystatus, sizeof(data.mystatus)); // Фиксируем статус b'calibra', чтобы на стороне ПК сработало ПО
      //currCommand = "moving";
      currCommand = "calibration";
      movingTime = millis();    // Фиксируем время начала движения
      data.time_ms = movingTime;
      prevT = movingTime;     // Чтобы ПИД
      Eintegral = 0;            // Чтобы ПИД
      Eprev = E;
      lastPidTime = movingTime; // Фиксируем время последнего срабатывания PID
      
      startM1Speed = m1Speed; //m1LightSpeed
      startM2Speed = m2Speed;
        // Скорости, на которых тронулись
      str = "m1Speed="; 
      str += String(m1Speed);  str += ", ";
      
      str += "m2Speed="; 
      str += String(m2Speed); //str += "\r\n";
    
    
    str.toCharArray(data.mytext, sizeof(data.mytext));
    
    data.M1_Speed = startM1Speed;
    data.M2_Speed = startM2Speed;
      
    //Serial.println(str);
    Serial.write((byte*)&data, sizeof(data));
    Serial.flush();


      }
//++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++
    if (message.startsWith("setSpeedInit")){ // Задаем начальные значения скоростей в режиме "на вису"
      // setSpeedInit 24 27
      currCommand = "setSpeedInit";
      str = message.substring(13); //sizeof(currCommand) + 1
      int N = separate (str, sPtr, SPTR_SIZE,  &strData); 
      String K;

      

      K = sPtr[0];
      m1LightSpeed = K.toInt();

      K = sPtr[1];
      m2LightSpeed = K.toInt();

      data.M1Speed_start = m1LightSpeed;
      data.M2Speed_start = m2LightSpeed;

      str = "Changed m[12]LightSpeed values ";
      str += String(m1LightSpeed);
      str += ", ";
      str += String(m2LightSpeed);
      str.toCharArray(data.mytext, sizeof(data.mytext));
      //Serial.println(str);
      str = "done";
      str.toCharArray(data.mystatus, sizeof(data.mystatus));

      Serial.write((byte*)&data, sizeof(data));
      Serial.flush();
 
      updateLimits();
      
      }
//++++++++++++++++++++++++++++++++++++++++++++++

      
    }//if (serial.available())
    
/*
    switch (data) {

    case 0x55:
        clamp();
        break;
    default:
        message = "Wrong data !!!";
        Serial.println(message);
        Serial.flush();
    }
*/
}//parse_command
//++++++++++++++++++++++++++++++++++++++
void decreaseSpeed(){
    while (m1Speed > movingThreshold || m2Speed > movingThreshold){
    m1Speed -=1;
    if (m1Speed < movingThreshold) {m1Speed = movingThreshold;}

    m2Speed -=1;
    if (m2Speed < movingThreshold) {m2Speed = movingThreshold;}

    md.setM1Speed(m1Speed);
    md.setM2Speed(m2Speed);
    delay(250);
    
    
  } // while threshold

  m1Speed = 1.1*m1Speed;
  m2Speed = 0.9*m2Speed;

  md.setM1Speed(m1Speed);
  md.setM2Speed(m2Speed);


  } // decreaseSpeed()
//++++++++++++++++++++++
void startIfStopped(){
          posAm1_prev = posAm1;
          posAm2_prev = posAm2;

          while ((posAm1 - posAm1_prev) < 50){

            m1Speed += 1;
            m2Speed += 1;
            md.setM1Speed(m1Speed);
            md.setM2Speed(m2Speed);

            delay(400);
            
          }

          str = "M++ speed = ";
          str += String(m1Speed);

          Serial.println(str);
          Serial.flush();
          currCommand = "moving";
          
          bool finished = false;
          while (!finished){
          m1Speed -= 1;
          m2Speed -= 1;
          if (m1Speed > startM1Speed) {md.setM1Speed(m1Speed);}
          if (m2Speed > startM2Speed) {md.setM2Speed(m2Speed);}

          if ((m1Speed <= startM1Speed) && (m2Speed <= startM2Speed)) {finished = true;}
          delay(250);

        }

  
  } // startIfStopped
//++++++++++++++++++++++
ISR(TIMER1_COMPA_vect)
{
// Считываем значения энкодеров, выводим в serial port
// Вычисляем разницу в данных энкодеров для дальнейшего принятия решения об
// изменении скорости моторов
//  getValues(); // итак выводится в loop


 
TCNT1  = 0;


//getValues(t1, t2);
int delta = 0;

int diff = posAm1 - posAm2 ;

//if (diff<0){delta= -1*diff;}
//else {
//  delta = diff;
//  }
//
//  // Пора ли включать ПИД ?
//  if (delta >=  encodersGAP){
//    pid1stTime = true;
// //   Serial.println("START PID !!!");
//    pid();
//    pid1stTime = false;
//    }
//  else{
//    getValues(t1, t2);
//    //prevT = micros();
//    Eprev = diff;
//  }
  int delta_A1, delta_A2; // разница с прошлым значением
    delta_A1 = abs(posAm1 - TimerA1_prev);
    delta_A2 = abs(posAm2 - TimerA2_prev); //  || delta <=5

    if ((delta_A1 ==0 || delta_A2 ==0) && (currCommand == "moving")){
//        Serial.println("Moving !");
//        Serial.flush();

      // Дальше проверяем число срабатываний
      if (stopCounter < 3){ 
        stopCounter += 1;
        str = "stopCounter = ";
        str += String(stopCounter);
        
//        Serial.println(str);
//        Serial.flush();

       }
      else {
        stopCounter = 0;
      // Значит ли, что мы стоим ?
      // Не факт, т.к. значения *_prev только что могли обновиться в loop.
      // Тогда нужно, чтобы оно сработало ПОДРЯД раза 3.
      // Это срабатывает, когда в pid() видим, что скорость слишком большая, тогда currCommand == "stop"
      // А если0( currCommand == "start"), то ничего не делаем, ф-ция startPlatform(); уже запущена
//      Serial.println("START Platform !!!");
      //startPlatform();

        str = "TIME = "; 
        str += String(millis());
        str += ", ";
        
        str += "posAm1=";
        str += String(posAm1); str += ", ";
        str += "posAm1_prev=";
        str += String(posAm1_prev); str += ", ";

        
        str += "posAm2=" ;
        str += String(posAm2); str += ", ";
        str += "posAm2_prev=" ;
        str += String(posAm2_prev); str += ", ";
        
        str += "diff="; 
        str += String(diff); str += ", ";
        
        str += "diff_prev=";
        str += String(diff_prev); str += ", ";
      
        str += "VectorDiff=";
        str += String(abs(diff-diff_prev)); str += ", ";
      
       
        
        str += "m1Speed=";
        str += String(m1Speed);  str += ", ";
        
        str += "m2Speed=";
        str += String(m2Speed); str += ", Stopped !";

      
//      Serial.println(str);
//      Serial.flush();
      //startIfStopped();
      } // else stopCounter
    }

//  }// else

  
   TimerA1_prev = posAm1;
   TimerA2_prev = posAm2;

} // ISR(TIMER1_COMPA_vect)

//++++++++++++++++++++++++++++++++
void updateLimits(){ //  Меняем значения верхнего и нижнего предела скоростей.
  speedBottomLimit = round(0.7*min(m1LightSpeed, m2LightSpeed));// 28; 
  speedTopLimit = round(2.1*min(m1LightSpeed, m2LightSpeed));//150;

}
