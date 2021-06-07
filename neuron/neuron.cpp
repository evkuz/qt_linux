#include "neuron.h"


//        Механизм запрос - ответ

void MainWindow::try_mcinfer(void){

            in[0] = actAngle;
            in[1] = e_Arr_Max;
            in[2] = de;

            MCInfer(&model,in,outKB);

            Kp = outKB[0];
            Ki = outKB[1];
            Kd = outKB[2];

            QString str = "The application ";
            str += QString::number(Kp);
            str += " ";
            str += QString::number(Ki);
            str += " ";
            str += QString::number(Kd);
            str += " ";

            Robot->Write_To_Log(0xf000, str.append(" \n"));

}
