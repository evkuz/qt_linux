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
  int E;                // 2 bytes
  int Eprev;            // 2 bytes
  int lagSpeed_ptr;          // 2 bytes
  int fwdSpeed_ptr;         // 2 bytes
  int M1Speed_ptr;          // 2 bytes
  int M2Speed_ptr;          // 2 bytes
  
  float Integral;           // 4 bytes
  char mystatus[8];         // 8 bytes string interpreting current device status
  unsigned long timestamp;  //4 bytes
  
}; //48 bytes total

// running
// stopped

volatile Enc data;

DualVNH5019MotorShield md; // Motor Driver

#define sBufSize 32   // Размер буфера компорта в плате NANO - 64 байта.
#define szParcel 8    // Размер посылки по Serial port

char buf[sBufSize];

byte ints[sBufSize]; // Данные, полученные по serial


String str="";
String currCommand = ""; // Текущая команда

const  byte pinAm1 = 19; // Пины драйвера Polulu
const  byte pinBm1 = 18; // 

const int defaultMSpeed = 25;
const int cruiseMSpeed = 50;
const int speedBottomLimit = 15; 
const int speedTopLimit = 150;

volatile int m1Speed = defaultMSpeed;
volatile int m2Speed = defaultMSpeed;

volatile int startM1Speed;
volatile int startM2Speed;


volatile int *fwdmSpeed; // Скорость опережающего
volatile int *lagmSpeed; // Скорость отстающего колеса


volatile long posAm1 = 0;
volatile unsigned long posBm1 = 0;

volatile int diameter; // 90mm
volatile int m1A_count, m1B_Count, m2A_count, m2B_count;
volatile double m1_count, m2_count;
volatile int m1A_k, m1B_k, m2A_k, m2B_k; // число отсчетов энкодера на 1 оборот колеса. Здесь k - коэффициент

// Возможно это дубль. Оставляем, чтобы компилятор проходил.
volatile long diffAbsolute = 0; // Абсолютная разница в энкодерах 1-го и 2го колеса 
volatile long diffRelative = 0; // Относительная разница(относительно предыдущих показаний) в энкодерах 1-го и 2-го колеса.

//+++++++++++++++++++++++++++++++++++++++++ PID variables +++++++++++++++++++
// volatile double Kp, Ki, Kd;
volatile double P, I, D;

const int timerPerRotation = 2; // Сколько раз срабатывает таймер за 1 оборот колеса
volatile double TRC = (double)1/timerPerRotation; // timer/rotation coefficient == dt т.к. таймер за 1 сек.


boolean isSomeOneLeading; // Флаг смены отстающего колеса на ведущее. Если М1_А > M2_A isSomeOneLeading == false, т.к. обычно М1 крутится быстрее М2
                         // Если отстающее колесо обгоняет ведущее, то значит меняем их статусы на противоположные.
                         // При этом скорость колес будет нарастать.
                         // Значит скорость сбрасываем, когда разница в показаниях меньше 


volatile long prevT = 0;    // for delta t counting
volatile int Eprev = 0;     // for delta Error counting

int E;

volatile long myprevT = 0;

unsigned long msPrevT = 0; // Для фиксации интервалов проверки E
unsigned long msCurrT = 0;

unsigned long movingTime = 0;
unsigned long elapsedTime = 0;

const unsigned long measureTime = 6000; // Время движения в мс
const unsigned long pidTime = 400; // Интервал в мс между корректировками скорости.

volatile float Eintegral = 0;

volatile byte encodersGAP = 30;                               // То включаем ПИД

bool pidFlag = false; // Флаг запуска ПИД    

long Timer1A_OneSec = 15624;

float t1 = 0.521; // test
float t2 = 15.125;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

volatile long posAm1_prev = 0; // previous value of Am1
volatile long posAm2_prev = 0; // previous value of Am2

volatile int diff = 0; // difference between posAm1, posAm2
volatile int diff_prev = 0; // previous difference between posAm1, posAm2


const  byte pinAm2 = 20;  //(PE4 for ATMega2560, Digital 2-pin)
const  byte pinBm2 = 21;  //(PE5 for ATMega2560, Digital 3-pin)
volatile long posAm2 = 0;
volatile unsigned long posBm2 = 0;

const int M1 = 480; // Мотор М1 Число отсчетов энкодера на 1 оборот колеса
const int M2 = 482; // Мотор М2 Число отсчетов энкодера на 1 оборот колеса
volatile long intM2counter = 0;
volatile long intM2Bcounter = 0;

volatile int pinAm1Value = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int pinBm1Value = 0;   // Переменные хранящие состояние пина, для экономии времени

volatile int pinAm2Value = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int pinBm2Value = 0;   // Переменные хранящие состояние пина, для экономии времени

volatile bool startFlag = false; // Началось движение тележки

const unsigned long ocrValue = 3906; // 46872 == 3c. 31248==2c 15624==1c 7812==0.5c 3906==0.25c 1953 = 0.125c 976 = 60ms

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

int dtmsplit(char *str, const char *delim, char ***array, int *length); 

int
separate (
    String str,
    char   **p,
    int    size );



//++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setup()
{
 // pinMode(LED_BUILTIN, OUTPUT);

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



  
  Serial.begin(115200); // Serial Speed, default baud rate for rosserial_python
  Serial.println("Timer settings test"); //Dual VNH5019 Motor Shield
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

 if (currCommand == "moving" || currCommand == "pid"){// Считаем время движения, если истекло - останавливаем.
  msCurrT = millis();
  elapsedTime = msCurrT - movingTime;

//  str = "ElapsedTime= ";
//  str += String(elapsedTime); str += ", ";
//  str += "movingTime= ";
//  str += String(movingTime); str += ", ";
//  
//    Serial.println(str);
//    Serial.flush();


  if (elapsedTime >= pidTime){ // Is it time to start PID ?
    int diff = posAm1 - posAm2 ;
    int delta = 0;

    if (diff<0){delta= -1*diff;}
    else {
      delta = diff;
      }
      
 //     pid();
      
    if (delta >= encodersGAP){ //delta >= encodersGAP && 
            pid();
            //pidFlag = true;
      }

      
      
    } //  (elapsedTime >= pidTime)
    
  if (elapsedTime >= measureTime){ // Истекло, встаем
    stopPlatform();
    currCommand = "finish--";


    
    
//    str = "posAm1="; 
//    str += String(posAm1); str += ", ";
//    str += "posAm2=" ; 
//    str += String(posAm2); str += ", ";
//    str += "Time_ms=" ; 
//    str += String(elapsedTime); str += ", ";
//    str += "startM1Speed=";
//    str += String(startM1Speed); str += ", ";
//    str += "startM2Speed=";
//    str += String(startM2Speed); str += ", ";
//    str += "E="; 
//    str += String(E); 
//    
//    str += "\n";


//    data.A1_Enc = posAm1;
//    data.A2_Enc = posAm2;
//
//    data.M1_Speed = m1Speed;
//    data.M2_Speed = m2Speed;
//    data.time_ms = elapsedTime; //movingTime;
//    data.deltaT = 0.555; // 1234.5678; 
//    data.dedt = 0.4444; //3.5689;  
//    data.E = E;
//    data.Eprev = Eprev;
//    
//    data.lagSpeed_ptr = m1Speed;
//    data.fwdSpeed_ptr = m2Speed;
//    data.M1Speed_ptr = &m1Speed;
//    data.M2Speed_ptr = &m2Speed;
//  
//    data.Integral = 0.0;
//    
//    currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));
//
//    data.timestamp = millis();
//
//    pidFlag = false;
//    movingTime = 0;
//
//    Serial.write((byte*)&data, sizeof(data));
//    Serial.flush();
//
   
    
//    Serial.println("Time !!!");
//    Serial.flush();
//    reset_All();
    Eprev = E;
    posAm1_prev = posAm1;
    posAm2_prev = posAm2;
    //myprevT = millis();
 //   delay(200);

    } //(elapsedTime >= measureTime)
  
  } // (currCommand == "moving";)

  if (currCommand == "finish--"){ // Проверяем полную остановку
    if (posAm1 == posAm1_prev && posAm2 == posAm2_prev){
      currCommand = "stopPOSA";
      data.A1_Enc = posAm1;
      data.A2_Enc = posAm2;
      E = posAm1 - posAm2;
      data.E = E;
      data.Integral = 333.0;
      data.time_ms = millis();
      currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));
      Serial.write((byte*)&data, sizeof(data));
      Serial.flush();
      reset_All();
      delay(500);
      }
    else {
            posAm1_prev = posAm1;
            posAm2_prev = posAm2;

      }
    }
 
   
delay(10);
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
  diff_prev = posAm1_prev - posAm2_prev;



  diff = posAm1 - posAm2;

  data.diff = diff;
  data.diff_vector = diff-diff_prev;
  
//  str = "posAm1=";
//  str += String(posAm1); str += ", ";
//  str += "posAm2=" ;
//  str += String(posAm2); str += ", ";
  
//  str += "diff="; 
//  str += String(diff); str += ", ";
//  
//  str += "diff_prev=";
//  str += String(diff_prev); str += ", ";
//
//  str += "VectorDiff=";
//  str += String(abs(diff-diff_prev)); str += ", ";
//
// 
//  
//  str += "m1Speed=";
//  str += String(m1Speed);  str += ", ";
  
//  str += "m2Speed=";
//  str += String(m2Speed); str += ", ";

  // Однако, время заложено в deltaT
//  str += "millis= ";
//  str += String(millis());
//  str += '\n';
  
  data.A1_Enc = posAm1;
  data.A2_Enc = posAm2;

  data.M1_Speed = m1Speed;
  data.M2_Speed = m2Speed;
  data.time_ms = millis();
  data.deltaT = delta_T; // 1234.5678; 
  data.dedt = dedt; //3.5689;  
  data.E = posAm1 - posAm2;
  data.Eprev = Eprev;
  
  data.lagSpeed_ptr = &m1Speed;
  data.fwdSpeed_ptr = &m2Speed;
  data.M1Speed_ptr = &m1Speed;
  data.M2Speed_ptr = &m2Speed;

  data.Integral = 0.0;
  
  //Serial.write((byte*)&data, sizeof(data));
  Serial.println(str);
  Serial.flush();
 
  
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
void startPlatform(){

  reset_All();
  int posA1;
  int posA2;
          //Останавливаем таймер
        cli();  // отключить глобальные прерывания
        OCR1A = ocrValue ; 
        TCNT1 = 0;
        TCCR1B=0x08; // Timer is stopped while CS1[2:0]==0
        TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
        sei(); // включить глобальные прерывания

       
        md.setM1Speed(defaultMSpeed);
        md.setM2Speed(defaultMSpeed);
        delay(70);

        float f1Speed = 0.0;
        float f2Speed = 0.0; // вещественное значение Скорости
        posA1 = posAm1;
        posA2 = posAm2;
 //       cli();
        while ((posA1 < 200) || (posA2 < 200))
        {
          delay(5);
         // Если не сдвинулся, то увеличиваем скорость
          f1Speed += 0.2; // 1.1*m1Speed;
          f2Speed += 0.2; // 1.1*m2Speed;

          m1Speed = defaultMSpeed +  round(f1Speed);
          m2Speed = defaultMSpeed +  round(f2Speed);
          
          md.setM1Speed(m1Speed);
          md.setM2Speed(m2Speed);
                    
          delay(5);
          posA1 = posAm1;
          posA2 = posAm2;
   
        }
       delay(50);

//       m1Speed = 0.8*m1Speed;
//       m2Speed = 0.8*m2Speed;
//       md.setM1Speed(m1Speed);
//       md.setM2Speed(m2Speed);

       
//    sei();
        diff_prev = abs(posAm1 - posAm2);
        currCommand = "movingNow";
        

        posAm1_prev = posAm1;
        posAm2_prev = posAm2;


        // Девайс тронулся, включаем таймер
        // Запускаем таймер
        cli();  // отключить глобальные прерывания
        OCR1A = ocrValue ; 
        TCNT1 = 0;
        TCCR1B=0x0D;
        TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
        sei(); // включить глобальные прерывания


  
  } // startPlatform()
//+++++++++++++++++++++++++++++++++++++
// Обычный старт, но без таймера
void startNoTimer(){
  int posA1;
  int posA2;

  md.setM1Speed(defaultMSpeed);
  md.setM2Speed(defaultMSpeed);
  delay(70);

  float f1Speed = 0.0;
  float f2Speed = 0.0; // вещественное значение Скорости
  posA1 = posAm1;
  posA2 = posAm2;
 //       cli();
  while (((posA1 < 200) || (posA2 < 200)) || ((m1Speed < cruiseMSpeed) || (m2Speed < cruiseMSpeed)) )
  {
    delay(5);
   // Если не сдвинулся, то увеличиваем скорость
    f1Speed += 0.2; // 1.1*m1Speed;
    f2Speed += 0.2; // 1.1*m2Speed;
  
    m1Speed = defaultMSpeed +  round(f1Speed);
    m2Speed = defaultMSpeed +  round(f2Speed);
    
    md.setM1Speed(m1Speed);
    md.setM2Speed(m2Speed);
    delay(5);
    posA1 = posAm1;
    posA2 = posAm2;
  
  }
  delay(50);
//  m1Speed = 0.6*m1Speed;
//  m2Speed = 0.6*m2Speed;
  
  startM1Speed = m1Speed;
  startM2Speed = m2Speed;
  
//  md.setM1Speed(m1Speed);
//  md.setM2Speed(m2Speed);


//        str = "m1Speed=";
//        str += String(m1Speed); str += ", ";
//        str += "m2Speed=";
//        str += String(m2Speed); str += "\n";
//        Serial.println(str);
//        Serial.flush();

  } //  startNoTimer()
//+++++++++++++++++++++++++++++++++++++
void stopPlatform(){
        currCommand = "stopped!";
        startFlag==false; // Готовы к старту.
        pidFlag = false;  // Останавливаем ПИД
        //reset_All();
        TIMSK1 &= ~(1 << OCIE1A);  // выключение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
        md.setM1Speed(0);
        md.setM2Speed(0);
//        Serial.println("Stopped");
//        Serial.flush();

  } //stopPlatform()
//+++++++++++++++++++++++++++++++++++++
void parse_command ()
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
        currCommand = "start";
        reset_All();

        startPlatform();
        
      } //if (message == "start")
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (message == "stop") // Запрещаем прерывания таймера 1 - перестаем слать данные в Serial Port
      {
        currCommand = "stop";
        startFlag==false; // Готовы к старту.
        pidFlag = false;  // Останавливаем ПИД
        //reset_All();
        TIMSK1 &= ~(1 << OCIE1A);  // включение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
        md.setM1Speed(0);
        md.setM2Speed(0);
//        Serial.println("Stopped");
//        Serial.flush();
        }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (message.startsWith("setPID")){ // Задаем PID-коэффициенты
      str = "Got PID changing";
      str = message.substring(7);
      
      Serial.println(str);
      Serial.flush();

      }
    

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

      if (message == "moveon") // Запускаем тележку на 5с., останавливаем, передаем показатели в ПК
      {
        
       reset_All();
       // Перед запуском двигателей сохраняем текущие значения как предыдущие
      
       startNoTimer();
       movingTime = millis(); // Фиксируем время начала движения
       currCommand = "moving";
       
       startFlag = true;

        data.A1_Enc = posAm1;
        data.A2_Enc = posAm2;
        
        data.M1_Speed = m1Speed;
        data.M2_Speed = m2Speed;
        data.time_ms = movingTime;
        data.deltaT = 0.005; // 1234.5678; 
        data.dedt = 0.0; //3.5689;  
        data.E = posAm1 - posAm2;
        data.Eprev = 0;
        
        data.lagSpeed_ptr = m1Speed;
        data.fwdSpeed_ptr = m2Speed;
        data.M1Speed_ptr = &m1Speed;
        data.M2Speed_ptr = &m2Speed;
        
        data.Integral = 777.0;
        
        currCommand.toCharArray(data.mystatus, sizeof(data.mystatus));
        data.timestamp = movingTime;

    Serial.write((byte*)&data, sizeof(data));
    Serial.flush();

//  Serial.println("Start Moving !!!" + String(movingTime));
//  Serial.flush();
       
//       cli();  // отключить глобальные прерывания
//       TCNT1 = 0;
//       TCCR1A=0;
//       TCCR1B=0x0D;
//       TIMSK1 |= (1 << OCIE1A);  // включение прерываний по совпадению или "сброс по совпадению", бит OCIE1A - Timer/Counter1, Output Compare A Match Interrupt Enable
//       sei(); // включить глобальные прерывания    

//        md.setM1Speed(m1Speed);
//        md.setM2Speed(m2Speed);
//
//        m1Speed = 0.3*m1Speed;
//        m2Speed = 0.3*m2Speed;
    
//        Serial.flush();
          return;
     } //"moveON"

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
          
          str = "posAm1="; 
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
      
//++++++++++++++++++++++++++++++++++++++++++++++
        
      
    }//if (serial.available())
    
//    delay(100);

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
//++++++++++++++++++++++
ISR(TIMER1_COMPA_vect)
{
// Считываем значения энкодеров, выводим в serial port
// Вычисляем разницу в данных энкодеров для дальнейшего принятия решения об
// изменении скорости моторов
//  getValues(); // итак выводится в loop

  int delta_A1, delta_A2; // разница с прошлым значением
  byte movingThreshold = 15;

TCNT1  = 0;


// getValues(t1, t2);

int diff = posAm1 - posAm2 ;
int delta = 0;

if (diff<0){delta= -1*diff;}
else {
  delta = diff;
  }

  if (delta >=  encodersGAP){
    pidFlag = true;
 //   Serial.println("START PID !!!");
    pid();
    pidFlag = false;
    }
  else{
    getValues(t1, t2);
    prevT = micros();
    Eprev = diff;

    delta_A1 = abs(posAm1 - posAm1_prev);
    delta_A2 = abs(posAm2 - posAm2_prev);

    if ((delta_A1 <=5 || delta_A2 <=5 || delta <=5) && (currCommand == "stop")){ // Значит мы стоим ! Это срабатывает, когда в pid() видим, что скорость слишком большая, тогда currCommand == "stop"
      // А если0( currCommand == "start"), то ничего не делаем, ф-ция startPlatform(); уже запущена
//      Serial.println("START Platform !!!");
      startPlatform();
      }

  }// else

  
    posAm1_prev = posAm1;
    posAm2_prev = posAm2;
    

} // ISR(TIMER1_COMPA_vect)

//++++++++++++++++++++++++++++++++
