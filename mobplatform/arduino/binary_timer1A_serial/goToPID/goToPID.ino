//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Головная ф-ция PID
// ОТстающее - ускоряем, обгоняющее - замедляем

void goToPID()
{
//  str = "I'm in PID-function !";
//  write2chatter(str);
  double backlog, advanced ;
  double Derror, DprevError;
  double Kp, Ki, Kd;

    Kp = 10.0; // В единицах скорости(сила тока)
    Ki = 1.0;
    Kd = 0.25;

   int  newSpeedAdvanced, newSpeedLag;
   
    int* advancedM; // Адрес скорости обгоняющего колеса
    int* advancedMxA_k;      // Адрес коэффициента MxA_k, x=[1,2]

   volatile int* speedLagM;     // Адрес скорости отстающего колеса
   volatile int* speedLagMxA_k; // Адрес коэффициента MxA_k, x=[1,2]
   volatile int mxA, MxSpeed;  // encoder_A and speed

    if (posAm1 <= posAm2){ // M1 is lag behind so correct M1 speed

        speedLagM = &m1Speed;
        *speedLagMxA_k = m1A_k;

//        *advancedM = m2Speed; // М2 обгоняет
//        *advancedMxA_k = m2A_k;       // коэфиициент MxA_k обгоняющего колеса.

    }

    if (posAm2 < posAm1) {// M2 is lag behind so correct M2 speed

        speedLagM = &m2Speed;
        *speedLagMxA_k = m2A_k;

//        *advancedM = m1Speed; // М1 обгоняет
//        *advancedMxA_k = m1A_k;

    }

//++++++++++++++ Отстающее колесо ускоряем

//double delitel = (double)*speedLagMxA_k*TRC;
//backlog = (double)(abs(diffAbsolute - encodersGAP))/delitel;

// int target = encodersGAP;

// Разница с "уставкой", это и есть error. error = target - measured

// Наш target - это encodersGAP
// ...
// encodersGAP набегает за 1 оборот колеса(примерно 1с) на скорости 100, сюда мы попадаем за время  deltaT


 // Если < 0 то, пора замедляться...

  diffAbsolute = abs(posAm1 - posAm2);
  diffRelative = diffAbsolute - encodersGAP;


// Разница в энкодерах незначительная, поэтому сбрасываем значения скоростей обоих колес до начального.
// И выходим !!! Не нужно ПИД запускать.
if ((diffAbsolute < encodersGAP) && isSomeOneLeading) { // Уже догнали, slowdown the speed, make it equal to default value
  isSomeOneLeading = false;
  m1Speed = defaultMSpeed;
  m2Speed = defaultMSpeed;
  return;
  }



// calculate dt
long currentT = micros();
// get microseconds

// Микросекунды делим на 1.0e6, получаем секунды.
// Иногда значения получаются отрицательные... счетчик переполнился ? 
float deltaT = ((float)(currentT - prevT))/(1.0e6);

mxA = *speedLagMxA_k; // Отстающее колесо, число отсчетов на 1 оборот
MxSpeed = *speedLagM;

// E - ошибка в разнице отсчетов для М1 и М2 
// float  E = (float)abs(abs(posAm1 - posAm2) - encodersGAP)*(float)1/mxA*deltaT*((float)MxSpeed/(float)defaultMSpeed);

// Немного упростим, код abs(abs(posAm1 - posAm2) - encodersGAP) отрабатывает неверно
float  E = (float)abs(abs(posAm1 - posAm2) - encodersGAP)/(float)mxA;

// I-составляющая
I = round(I + (float)E*deltaT);

str = "MxSpeed ";
str += String(MxSpeed); str += ", ";

str += "deltaT(us)=";
str += String(deltaT,4); str += ", ";

str += "E = ";
str += String(E,4); str += ", ";
//write2chatter(str);

// if (E < encodersGAP) {return;}
str += "I=";
str += String(I); //str+=", ";


prevT = currentT;

// Derivative
float dedt = (E - Eprev)/deltaT;

//D = (Derror - DprevError)/TRC; // 0.5c - примерное время таймера, это наше dt - интервал интегрирования
// Предыдущая разница с "уставкой"
Eprev = E;

// Integral


// newSpeedspeedLag = round((double)(*speedLagM)*(1+backlog)) + Kd*D;
// mxA - число отсчетов энкодера за 1 оборот, а т.к. за время deltaT меньше 1 оборота (на скорости defaultMSpeed) 
// То берм отношение 1с к deltaT, также скорость не всегда defaultMSpeed, поэтому берем отношение *speedLagM к defaultMSpeed

//Kp = (double)(1/mxA)*deltaT*(double)(*speedLagM/defaultMSpeed);


// Get new speed value, corrected by PID, as u(t)
// Управляющее воздействие в нашем случае - скорость.

newSpeedLag = round((float)Kp*E + (float)Ki*I + (float)Kd*dedt); // Это типа deltaSpeed ? Управляеющее воздействие...

float aaa = (float)Kp*E;
//str = "Kp*E=";
//str += String(aaa,4); str+=", ";
//
//aaa = (float)E*deltaT;
//str += "E*deltaT=";
//str += String(aaa,4); str+=", ";
//
//
//aaa = (float)Kd*dedt;
//str += "Kd*dedt=";
//str += String(aaa,4); str+=", ";

//str = "float Ki*I=";
//str += String((float)Ki*I,4); str+=", ";
//write2chatter(str);
//

//str = "float Kd*dedt=";
//str += String((float)Kd*dedt,4) ;
//
//write2chatter(str);

str = "PID(u)==newSpeedLag = ";
str += String(newSpeedLag);
//write2chatter(str);


//int qqq = *speedLagM;

// Увеличиваем для отстающего
//str = "*speedLagM before PID = ";
//str += String(qqq); //str+=", ";
//write2chatter(str);
//newSpeedLag = 1;

 *speedLagM = MxSpeed + newSpeedLag;// и вот тут у нас НУЛЬ !
//*speedLagM += newSpeedLag;

str = "m1Speed ";
str += String(m1Speed); str += ", ";
str += "m2Speed ";
str += String(m2Speed); 
//write2chatter(str);

if ((abs)(m1Speed - m2Speed) > 100){
   m1Speed = defaultMSpeed;
   m2Speed = defaultMSpeed;
   str = "!!! Speed values aligned !!!";
 //  write2chatter(str);
  }

  
//qqq = *speedLagM;

//str = "*speedLagM AFTER PID = ";
//str += String(qqq); //str+=", ";
//write2chatter(str);


} // goToPID  
