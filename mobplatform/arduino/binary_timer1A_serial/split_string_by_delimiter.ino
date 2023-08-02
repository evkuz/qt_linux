//#define SPTR_SIZE   20
//char   *sPtr [SPTR_SIZE];

int
separate (
    String str,
    char   **p,
    int    size )
{
    int  n;
    char s [100];

    strcpy (s, str.c_str ());

    *p++ = strtok (s, " "); //вот тут n == 0
    for (n = 1; (*p++ = strtok (NULL, " ")) != NULL; n++)
        if (n == size)
            break;

    return n;
}

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
