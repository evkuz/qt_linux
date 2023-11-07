#ifndef ROS_MOB_PLATFORM_DESCRIPTION_H
#define ROS_MOB_PLATFORM_DESCRIPTION_H
/*
 * mobPlatform package description
 * Node list :
 * 
 * Используем ноду serial_node.py из пакета rosserial_python
 * 
 * rosrun [package_name] [node_name]
 * 
 * rosrun rosserial_python serial_node.py /dev/ttyACM0
 * Но Узел serial_node.py принадлежит библиотеке rosserial_arduino/ros_lib, а не пакету rosserial_python...
 * Сие вопрос понимания...
 * Разбираемся
 * ...
 * 
 * Это узел для обмена данными с последовательным портом.
 * 
 * Узел выполняет следующий обмен данными через ROS (создает следующие топики) :
 * - Принимает (подписан на) сообщения из топика "mobplatform" (приём команд "снаружи" в сом-порт)
 *   subscriber on mobplatform [std_msgs/String]
 *   
 * - постит сообщения в топик  "encoders" (выдача данных энкодеров из сом-порта "наружу")
 *   publisher on encoders [std_msgs/String]
 *   
 * Соответственно, для запуска двигателей мобильной платформы отправляем сообщение ("start") в топик mobplatform.
 * Узел считывает данные из этого топика
 * 
 * 
 * Узел serial_node.py принадлежит библиотеке rosserial_arduino/ros_lib
 * 
 *<launch>
  <node pkg="rosserial_python" type="serial_node.py" name="serial_node">
    <param name="port" value="/dev/ttyACM1"/>
    <param name="baud" value="115200"/>
  </node>
</launch>
 *
 *
 *
  */
#endif // WORKFLOW_MOB_PLATFORM_H
