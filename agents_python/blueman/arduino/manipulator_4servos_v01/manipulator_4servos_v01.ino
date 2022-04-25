/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <EEPROM.h>

// Sonar variables
int trigPin = 53;    // Trigger
int echoPin = 52;    // Echo
int duration;
float sonarValue;


//Servo parameters
const int nServos = 5;
int i;

Servo srv[nServos];

int start[nServos] =  { 90, 120,  35, 90, 80};
int minPos[nServos] = {  0,   10,   0,   10,   40};
int maxPos[nServos] = {176, 160, 110, 180, 120};

int curPos[nServos];
int goalPos[nServos];

int speedValue;
int servoDelay = 40;
bool movingFlag = false;


// Serial port communication
const int input_buffer = 27;
char sdata[input_buffer];
char *pSdata;
byte ch;
int sdataLength;

char command = 'u';
int values[nServos + 1];


// ***********************************************
// B A S E   M E T H O D S

void setup()
{
  //init serial port
  Serial.begin(115200);
  pSdata = sdata;
  *pSdata = '\0';
  sdataLength = 0;

  Serial.print("N_SERVOS: ");
  Serial.print(nServos);
  Serial.print("\n");
  
  read_start_values();

  Serial.print("START_VALUES: ");
  for(i=0; i< nServos; i++) {
    Serial.print(*(start + i));
    Serial.print('\t');  
  }
  Serial.print("\n");
    
  for(i=0; i< nServos; i++){
    curPos[i] = start[i];
    srv[i].write(start[i]);
  }
  srv[0].attach(12);  // attaches the servo on pin 9 to the servo object
  srv[1].attach(11);
  srv[2].attach(10);
  srv[3].attach(9);
  srv[4].attach(6);

  //Define sonar inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

//--------------------------------

void loop()
{
  while (Serial.available()) {
    ch = Serial.read();
    if (ch == '\n') { // Command recevied and ready.
      *pSdata++ = '\0';

      apply_command();

      pSdata = sdata;
      *pSdata = '\0'; // Clear the string ready for the next command.
      sdataLength = 0;
      break;
    }

    // -1 for null terminator space
    if ((pSdata - sdata) >= (input_buffer - 1)) {
      Serial.print("BUFFER OVERRUN\n");
      continue;
    }
    *pSdata++ = (char)ch;
    sdataLength++;
  }

  if (movingFlag) {
    move_servos();
  }
  else {
    delay(50);
  }
}

//--------------------------------

void read_start_values(){
  int eeAddress = 0;
  for(i = 0; i < nServos; i++){
    EEPROM.get(eeAddress, start[i]);
    eeAddress += sizeof(int);
    
    if (start[i] > maxPos[i]) {
      start[i] = maxPos[i];
    } else if (start[i] < minPos[i]) {
      start[i] = minPos[i];
    }
  }
}


//====================================================

void apply_command() {
  command = *sdata;
  
  if (command == 's') {
      Serial.print("OK\n");
      print_state();
  } else if (command == 'm') {
    if(parse_command(nServos + 1)) {
      Serial.print("OK\n");
      set_start_moving(values, values[nServos]);
    }
    else {
      print_help();
    }
  } else if (command == 'r') {
    Serial.print("OK\n");
    set_stop_moving();
  } else if (command == 'h') {
    if(parse_command(1)) {
      Serial.print("OK\n");
      set_move_to_start(values[0]);
    }
    else {
      print_help();
    }
  } else if (command == 'e') {
    if(parse_command(nServos)) {
      Serial.print("OK\n");
      set_start_positions(values);
    } else {
      print_help();
    }
  } else {
    print_help();
  }
}



//====================================================
// P A R S I N G   C O M M A N D   L I N E

int beg, end, argc;
bool parse_command(int expectedArgCount) {
  argc = 0;
  beg = 2; // 0 - command, 1 - space or '\0', ...

  while (beg < sdataLength) {
    end = find_space(sdata, beg);

    if (end < 0) {
      end = sdataLength;
    }

    if (end - beg > 3){
      return false;
    }
      
    values[argc] = parse_int(sdata+beg, sdata+end-1);
    argc++;
    
    if (argc > nServos) {
      break;
    }
    
    beg = end + 1;
  }
  
  if(argc == expectedArgCount) {
    return true;
  }

  return false;
}

//--------------------------------

int find_space(const char* src, int beg) {
  char *pSrc = src + beg;
  while (true) {
    if(beg >= sdataLength)
      return -1;

    if (*pSrc == '\0')
      return -1;
      
    if (is_space(pSrc))
      break;
    beg++;
    pSrc++;
  }
  return beg;
}

//--------------------------------

bool is_space(char* c){
  if( *c == ' ' || *c == '\t')
    return true;
  return false;
}

//--------------------------------

char intBuf[5];
int parse_int(char *begin, char *end) {
  char *pos = begin;
  char *bufPos = intBuf;

  while (pos != (end + 1)) {
    *bufPos = *pos;
    pos++;
    bufPos++;
  }
  *bufPos = '\0';
  return atoi(intBuf);
}



//====================================================
// P R I N T S   I N F O

void print_help() {
  Serial.print("Wrong command!\n");
  Serial.print("Available commands:\n");
  Serial.print(" * 'm pos1 pos2 pos3 pos4 pos5 speedInPercentage\\n' - move servos to given positions\n");
  Serial.print(" * 's\\n' - return current positions\n");
  Serial.print(" * 'r\\n' - stop moving\n");
  Serial.print(" * 'e start1 start2 start3 start4 start5\\n' - set servos start positions\n");
  Serial.print(" * 'h speedInPercentage\\n' - go to start position\n");
}

//-----------------------------------

void print_state() {
  for(i=0; i< nServos; i++) {
    Serial.print(*(curPos + i));
    Serial.print('\t');  
  }

  update_sonar_value();
  Serial.print(sonarValue);
  Serial.print("\n");
}

//-----------------------------------

void update_sonar_value() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  // cm = (duration / 2) / 29.1;
  sonarValue = duration * 0.017182; //in cm
}



//====================================================
// M O V I N G   C O N T R O L

void set_move_to_start(int servoSpeed) {
  set_start_moving(start, servoSpeed);
}

//-----------------------------------

void set_start_moving(const int *newPos, int servoSpeed) {
  for(i=0; i< nServos; i++) {
    if (*(newPos + i) > maxPos[i]) {
      goalPos[i] = maxPos[i];
    } else if (*(newPos + i) < minPos[i]) {
      goalPos[i] = minPos[i];
    } else {
      goalPos[i] = *(newPos + i);
    }
  }
  
  movingFlag = true;
  set_speed(servoSpeed);
}

//-----------------------------------

void set_stop_moving() {
  movingFlag = false;
  Serial.print("END\n");
}

//-----------------------------------

/*
  Servo speed will be computed with this assumptions

  %  - delay
  100 -  1
  1  - 199

  x - speed, y - delay
  ax + b = y
  a = -2, b = 201
*/
void set_speed(int servoSpeed) {
  if (servoSpeed > 100) servoSpeed = 100;
  if (servoSpeed < 1) servoSpeed = 1;

  servoDelay = 201 - 2 * servoSpeed;
}

//-----------------------------------

void set_start_positions(const int *newPos) {
  int eeAddress = 0;
  for(i=0; i< nServos; i++) {
    if (*(newPos + i) > maxPos[i]) {
      start[i] = maxPos[i];
    } else if (*(newPos + i) < minPos[i]) {
      start[i] = minPos[i];
    } else {
      start[i] = *(newPos + i);
    }
    
    EEPROM.put(eeAddress, start[i]);
    eeAddress += sizeof(int);
  }
}



//====================================================
// M O V E   S E R V O S

bool isFinished;
void move_servos()
{
  isFinished = true;
  for(i=0; i < nServos; i++){
    curPos[i] = srv[i].read();
    if (curPos[i] > goalPos[i]) {
      curPos[i]--;
      srv[i].write(curPos[i]);
      delay (servoDelay);
      isFinished = false;
    } else if (curPos[i] < goalPos[i]) {
      curPos[i]++;
      srv[i].write(curPos[i]);
      delay (servoDelay);
      isFinished = false;
    }
  }
  
  if (isFinished) {
    set_stop_moving();
  }
}
