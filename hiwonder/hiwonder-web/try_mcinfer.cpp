// Выполняем целый набор действий - взять кубик и положить на транспортировщик
void MainProcess::on_trainButton_clicked()
{
    int value = 0xf014;
    DetectorState state;
    QString str, fstr;
    str = "";
    fstr = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    Servos[0]=0;

    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){

            str = "Just float values of state : ";
            fstr.setNum(state.objectX);
            str += fstr;
            str += ", ";
            fstr.setNum(state.objectY);
            str += fstr;
            GUI_Write_To_Log(0xf014, str);


            int kf = 100000;
            try_mcinfer(state.objectX*kf, state.objectY*kf); // Тут меняем current_status = "inprogress". Команда 0 - Переместить открытый хват к кубику.
            X = state.objectX;                        //  Хват открывается в процессе движения робота, а не отдельной командой.
            Y = state.objectY;
            std::cout << "DETECTED: " << state.objectX << " " << state.objectY << std::endl;
            //str += QString::number(state.objectX);
            fstr.setNum(state.objectX);
            str += fstr;
            str += ", ";
            //str += QString::number(state.objectY);
            fstr.setNum(state.objectY);
            str += fstr;
            DETECTED = true;

        } else {
            str += "NOT DETECTED";
            // change status to "NoDetection"
            jsnStore->currentStatus.rc = -5; // No Detection
            jsnStore->currentStatus.info = "No Detection";
            jsnStore->currentStatus.state = jsnStore->statuslst.at(jsnStore->INDEX_NODETECTION).toStdString();
            jsnStore->struc_2_jsnObj(); // Копируем данные структуры в ordered_json jsnStatus

            jsnStore->returnJsnStatus(); // Берем данные из ordered_json jsnStatus и передаем в QJsonObject::jsnObj

            jsnStore->setActionStart2NoDetection();
            str = jsnStore->returnJsnData();
            Write_2_TcpClient_Signal(str);

//            QString s3 = "NO DETECTION";
            str.insert(0, "NO DETECTION \n");
            GUI_Write_To_Log(value, str);
        }

       std::cout <<  str.toStdString() << std::endl;
       Robot->Write_To_Log(value, str);
       GUI_Write_To_Log(0xf014, str);
    }

//В этой точке робот опустился над кубиком, открыл захват.

if (DETECTED)
    {

        QByteArray dd ;
        dd.resize(parcel_size);
        str = "Next movement to robot";
        this->GUI_Write_To_Log (0xF055, str);
   //+++++++++++++++++ 1 make clamp, хватаем кубик
   //on_clampButton_clicked();
        Servos[0]=90;
        this->send_Data(NOT_LAST); // Движение "Туда" по умолчанию
   //++++++++++++++++++++ 2 make stand up, встаем в исходную точку
   //on_stand_upButton_clicked();
  // this->update_Servos_from_position(hwr_Start_position);
        memcpy(Servos, hwr_Start_position, DOF);
        this->send_Data(LASTONE);

   //+++++++++++++++++++++ 3 put the cube, наклоняем захват с кубиком к транспортировщику
   // {60, 93, 100, 35, 145, 35};
//   this->update_Servos_from_position(put_2_mobman);
//   memcpy(dd.data(), Servos, 6);
//   dd.insert(6, 0x31); // Движение "Туда"
//   this->send_Data(NOT_LAST); //BEFORE_LAST==0xE9, NOT_LAST ==C8
//   //+++++++++++++++++++++ 4  Unclamp the gripper, открываем захват, т.е. кладем кубик на пол, чтобы его взял транспортировщик
//   //on_clampButton_clicked();
//   Servos[0]=0;
//   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++ 5 Приподнять хват, чтобы не задеть тележку.
//      this->update_Servos_from_position(after_put_2_mobman);
//      memcpy(dd.data(), Servos, 6);
//      dd.insert(6, 0x30); // Движение "Туда"
//      this->send_Data(AFTER_PUT);



   //+++++++++++++++++++++ 6 go back to start position
   //this->update_Servos_from_position(hwr_Start_position);
   memcpy(Servos, hwr_Start_position, 6);
   memcpy(dd.data(), Servos, 6);
   dd.insert(parcel_size-2, 0x30); // Движение "Обратно"
   dd.insert(parcel_size-1, LASTONE);

   // this->ssend_Data(dd); // The last command
   Robot->GoToPosition(dd);

  }// if (DETECTED)

   DETECTED = false;

}//on_trainButton_clicked()
