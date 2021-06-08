#include "neuron.h"


//        Механизм запрос - ответ

void MainWindow::try_mcinfer(int x, int y){

//            in[0] = actAngle;
//            in[1] = e_Arr_Max;
//            in[2] = de;

            in[0] = x;
            in[1] = y;
            in[2] = 0;
            MCInfer(&model,in,outKB);

//            Kp = outKB[0];
//            Ki = outKB[1];
//            Kd = outKB[2];
            Servos[2] = outKB[0];
            Servos[3] = outKB[1];
            Servos[4] = outKB[2];
            Servos[5] = outKB[3];

            for (int i =0; i<= DOF -1; i++)
            {
                qle_list[i]->setText(QString::number(Servos[i]));
            }

    on_set_posButton_clicked();

    QString str = "Mcinfer Data : ";
    unsigned char* sData = reinterpret_cast<unsigned char*>(Servos);
    for (int i=0; i<= DOF - 1; i++){
        str += QString::number(sData[i]);
        str+= ", ";
    }
 Robot->Write_To_Log(0xf020, str.append(" \n"));
//            str += QString::number(Servos[2]);
//            str += " ";
//            str += QString::number(Servos[3]);
//            str += " ";
//            str += QString::number(Servos[4]);
//            str += " ";
//            str += QString::number(Servos[5]);
//            str += " ";



}
