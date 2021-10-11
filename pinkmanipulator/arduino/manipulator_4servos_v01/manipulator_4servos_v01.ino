/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

int DelayServo = 10;

int trigPin = 53;    // Trigger
int echoPin = 52;    // Echo
long duration, cm;

//задать параметры
const int start1=91;
const int start2=120;
const int start3=80;
const int start4=120;

int minPos1 = 0;
int maxPos1 = 180;
int minPos2 = 0;
int maxPos2 = 170;
int minPos3 = 0;
int maxPos3 = 180;
int minPos4 = 0;
int maxPos4 = 180;

int pos1 = start1;  // от 0 до 180
int pos2 = start2;  // от 0 до 170       //1 PID proportional control Gain 095;0115;001;
int pos3 = start3;
int pos4 = start4;  //180 - закрыто

Servo myservo1;  // нижние звено create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
Servo myservo3;  // create servo object to control a servo
Servo myservo4;  // create servo object to control a servo

#define BUF_LEN 18
char sdata[BUF_LEN];
char *pSdata;
byte ch;
bool gotCommand;

void setup() 
{
  myservo1.attach(12);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(11);
  myservo3.attach(10);
  myservo4.attach(9);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(115200);
  pSdata = sdata;
  *pSdata = '\0';
  gotCommand = false;
  ManipWrite(pos1, pos2, pos3, pos4);  
}


void loop()
{
  while(Serial.available()){
    ch = Serial.read();
    if (ch=='\n') {  // Command recevied and ready.
      *pSdata++ = '\0';

      if (sdata[0] == 'm') {
        if ((pSdata - sdata) == BUF_LEN) {
          // Process command in sdata.
          process_move_command();
          gotCommand = true;
        }
        else {
          Serial.print("Wrong command length!\n");
        }
      }
      else if (sdata[0] == 's') {
        gotCommand = true;
      }
      else {
        Serial.print("Wrong command!\n");
      }
      
      pSdata = sdata;
      *pSdata = '\0'; // Clear the string ready for the next command.
      break;
    }
    // -1 for null terminator space
    if ((pSdata - sdata) >= (BUF_LEN-1)) {
      Serial.print("BUFFER OVERRUN\n");
      continue; 
    }
    *pSdata++ = (char)ch;
    gotCommand = false;
  }

  if (gotCommand) {
    ManipWrite(pos1, pos2, pos3, pos4);
      
    Serial.print(pos1);
    Serial.print('\t');
    
    Serial.print(pos2);
    Serial.print('\t');
  
    Serial.print(pos3);
    Serial.print('\t');
  
    Serial.print(pos4);
    Serial.print('\t');
    
    if (is_near_cube()) {
      Serial.print("1\n");
    }
    else {
      Serial.print("0\n");
    }
    gotCommand = false;
  }
  else{
    delay(50);
  }
}

void process_move_command(){
  pos1 = parse_int((sdata + 2), (sdata + 4));
  pos2 = parse_int((sdata + 6), (sdata + 8));
  pos3 = parse_int((sdata + 10), (sdata + 12));
  pos4 = parse_int((sdata + 14), (sdata + 16));

  //проверка на допустимость
  if (pos1 > maxPos1) pos1 = maxPos1;
  if (pos1 < minPos1) pos1 = minPos1;

  if (pos2 > maxPos2) pos2 = maxPos2;
  if (pos2 < minPos2) pos2 = minPos2;

  if (pos3 > maxPos3) pos3 = maxPos3;
  if (pos3 < minPos3) pos3 = minPos3;

  if (pos4 > maxPos4) pos4 = maxPos4;
  if (pos4 < minPos4) pos4 = minPos4;
}

char intBuf[5];
int parse_int(char *begin, char *end){
  char *pos = begin;
  char *bufPos = intBuf;
  
  while(pos != (end+1)) {
    *bufPos = *pos;
    pos++;
    bufPos++;
  }
  *bufPos = '\0';
  return atoi(intBuf);
}

bool is_near_cube(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
    
  if (cm<4)
    return true;
  else
    return false;
}

void ManipWrite(int a, int b, int c, int d)
{
  ///////////////////////////////////////////////////////////////////////////
  if (myservo1.read() > a)
  { 
    for (int i = myservo1.read(); i > a; i--) {
      myservo1.write(i);
      delay (DelayServo);
    }
  }
  else if (myservo1.read() < a) 
  {  
    for (int i = myservo1.read(); i < a; i++) {
      myservo1.write(i);
      delay (DelayServo);
    }
  }
  ////////////////////////////////////////////////////////////////////////////
  if (myservo2.read() > b)
  { 
    for (int i = myservo2.read(); i > b; i--) {
      myservo2.write(i);
      delay (DelayServo);
    }
  }
  else if (myservo2.read() < b)
  {
    for (int i = myservo2.read(); i < b; i++) {
      myservo2.write(i);
      delay (DelayServo);
    }
  }
  ////////////////////////////////////////////////////////////////////////////
  if (myservo3.read() > c) 
  { 
    for (int i = myservo3.read(); i > c; i--) {
      myservo3.write(i);
      delay (DelayServo);
    }
  }
  else if (myservo3.read() < c)
  {
    for (int i = myservo3.read(); i < c; i++) {
      myservo3.write(i);
      delay (DelayServo);
    }
  }
  ////////////////////////////////////////////////////////////////////////////
  if (myservo4.read() > d) 
  { 
    for (int i = myservo4.read(); i > d; i--) {
      myservo4.write(i);
      delay (DelayServo);
    }
  }
  else if (myservo4.read() < d)
  {
    for (int i = myservo4.read(); i < d; i++) {
      myservo4.write(i);
      delay (DelayServo);
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
