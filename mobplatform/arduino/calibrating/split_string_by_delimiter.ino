






//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//#define SPTR_SIZE   20
//char   *sPtr [SPTR_SIZE];

int
separate (
    String str,
    char   **p,
    int    str_size )
{
    int  n;
    char s [100];

    strcpy (s, str.c_str ());

    *p++ = strtok (s, " "); //вот тут n == 0
    for (n = 1; (*p++ = strtok(NULL, " ")) != NULL; n++)
        if (n == str_size)
            break;

    return n;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int
EK_separate (
    String str,
    char   **p,
    int    str_size )
{
    int  n;
    char s [100];
    char *token;

    strcpy (s, str.c_str ());
    token = strtok (s, " ");
    n=0;
    while (token != NULL){
      *p++ = token;
      token = strtok (s, " ");
      n++;
      if (n == str_size)
         break;
      }

//    *p++ = strtok (s, " "); //вот тут n == 0
//    for (n = 1; (*p++ = strtok(NULL, " ")) != NULL; n++)
//        if (n == str_size)
//            break;

    return n;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int 
EK_split_str (
    String str,
    char   **p,
    int    str_size )
{
  int StringCount = 0;
  String strs[str_size];
  String mystr;
  String mydata;

  char **svPtr;

  svPtr = p;

  
   while (str.length() > 0)
  {
    int index = str.indexOf(' ');
    if (index == -1 && StringCount > 0)  // No space found
    {
     // strs[StringCount++] = str;
     StringCount++;
      mystr = str;
      const char *data = mystr.c_str();
      *p++ = data;
      str  = ""; // now str.length == 0
      mydata = "FINAL selected string is ";
      mydata += data;
      
      Serial.println (mydata);// data //sPtr [n]
      Serial.flush();

      //p--;
      
      break;
    }
    else
    {
      //strs[StringCount++] = str.substring(0, index);
      StringCount++;
      mystr = str.substring(0, index);
      const char *data = mystr.c_str();
      *p++ = data;
      str = str.substring(index+1);
      mydata = "selected string is ";
      mydata += data;
      mydata += ", ";
      mydata += "array data : ";
      mydata += *(p -1);
      mydata += ", ";
      mydata += "next string is ";
      mydata += str;
      Serial.println (mydata);// data //sPtr [n]
      Serial.flush();

    } // else
  } //while

  p = svPtr;
  for (int i =0; i< StringCount; i++){
    mydata = *((*p) + i);
    str = "dynamic array data ";
    str += mydata;
    Serial.println (str);// data //sPtr [n]
    Serial.flush();

    }
  return StringCount;
}// EK_split_str
//+++++++++++++++++++++++++++++++++++++++
// String  str= "A1N c2 t120 d4 t0 b5 t0 a2 t368 e2 t452 1c t0 e1 t600";

//void
//setup (void) {
//    Serial.begin (9600);
//
//    Serial.println (str);
//
//    int N = separate (str, sPtr, SPTR_SIZE);
//    for (int n = 0; n < N; n++)
//        Serial.println (sPtr [n]);
//}
//
