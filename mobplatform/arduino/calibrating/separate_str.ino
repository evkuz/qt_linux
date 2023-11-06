//https://forum.arduino.cc/t/how-to-split-a-string-with-space-and-store-the-items-in-array/888813/6
//#define SPTR_SIZE   20
//char *sPtr [SPTR_SIZE]; // this is reused each call
//char *strData = NULL; // this is allocated in separate and needs to be free( ) eventually
size_t numberOfStr = 0;  // number of valid elements in sPtr[  ]

// use this free the memory after use
void freeData(char **pdata) {
  free(*pdata);
  *pdata = NULL;
  numberOfStr = 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Однако, freeData(&strData); приводит к тому, что теряется элемент sPtr[0].
// А если сначала сохранить элемент sPtr[0] в другую переменную String, а потом уже freeData(&strData);,
// То все норм. При этом остальные элементы sPtr[] сохраняются.
// Это есть отдельная тема для исследований :)
int separate (String& str,  // pass as a reference to avoid double memory usage
              char **p,  
              int size, 
              char **pdata ) {
  int  n = 0;
  free(*pdata); // clean up last data as we are reusing sPtr[ ]
  // BE CAREFUL not to free it twice
  // calling free on NULL is OK
  *pdata = strdup(str.c_str()); // allocate new memory for the result.
  if (*pdata == NULL) {
    Serial.println("OUT OF MEMORY");
    return 0;
  }
  *p++ = strtok (*pdata, " ");
  for (n = 1; NULL != (*p++ = strtok (NULL, " ")); n++) {
    if (size == n) {
      break;
    }
  }
  return n;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//String  str1 = "A1N c2 t120 d4 t0 b5 t0 a2 t368 e2 t452 1c t0 e1 t600";
//String  str2 = "t0 a2 t368 e2 t452 1c t0 e1 t600";
//String  str3 = "0.0235 1.2345 222.22 345.32 7 550";

//void
//setup (void) {
//  Serial.begin (115200);
//  for (int i = 10; i > 0; i--) {
//    Serial.print(' '); Serial.print(i);
//    delay(500);
//  }
//  Serial.println();
//  strData = NULL; //make sure it is NULL to start with
//}

//void loop (void) {
//  String str;
//  int N = 0;
//  str = str1;
//  Serial.print(F("input str:")); Serial.println(str);
//  N = separate (str, sPtr, SPTR_SIZE,  &strData);
//  for (int n = 0; n < N; n++) {
//    Serial.println (sPtr [n]);
//  }
//  freeData(&strData);
//
//  str = str3;
//   Serial.print(F("input str3:")); Serial.println(str);
//  N = separate (str, sPtr, SPTR_SIZE,  &strData);
//  for (int n = 0; n < N; n++) {
//    Serial.println (sPtr [n]);
//  }
//  freeData(&strData);
//
//  
//  delay(6000);
//}
