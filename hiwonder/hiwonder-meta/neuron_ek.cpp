#include "neuron.h"
//        Механизм запрос - ответ

void MainProcess::try_mcinfer(float x, float y){
    QString str, fstr;

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
            Servos[2] = (unsigned char)outKB[0];
            Servos[3] = (unsigned char)outKB[1];
            Servos[4] = (unsigned char)outKB[2];
            Servos[5] = (unsigned char)outKB[3];
        // Ограничиваем значения
//unsigned char* sData = reinterpret_cast<unsigned char*>(Servos);
            for (int i =0; i<= DOF -3; i++)
            {
                if (Servos[i+2] > 180) {Servos[i+2]= 180;}
            }

        str = "NEURON data : ";
        for (int i =0; i<= DOF -3; i++)
        {
            str+= QString::number(outKB[i]);
            str += ", ";

        }

        Robot->Write_To_Log(0xf020, str.append(" \n"));

    Robot->SetCurrentStatus("inprogress");
    str = "Robot current status is ";
    str += Robot->GetCurrentStatus();
    Robot->Write_To_Log(0xf020, str);

    send_Data (NOT_LAST);


    str = "Mcinfer Data : ";
    fstr.setNum(x);
    str += fstr;
    str+= ", ";
    fstr.setNum(y);
    str += fstr;
    str+= ", ";


    for (int i=0; i<= DOF - 1; i++){
        str += QString::number(Servos[i]);
        str+= ", ";
    }
 Robot->Write_To_Log(0xf020, str.append(" \n"));



}
