#include "neuron.h"


//        Механизм запрос - ответ

void try_mcinfer(void){

            in[0] = actAngle;
            in[1] = e_Arr_Max;
            in[2] = de;

            MCInfer(&model,in,outKB);

            Kp = outKB[0];
            Ki = outKB[1];
            Kd = outKB[2];

}
