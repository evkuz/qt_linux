/*
 * Разные Углы начальной/стартовой позиции
 * Заданные положения манипулятора
 *
*/
#ifndef POSITIONS_H
#define POSITIONS_H

#include <QtCore>

unsigned char hwr_Start_position [6] = {93, 93, 93, 93, 93, 93}; // servo1,,,servo6
unsigned char sit_down_position [6] = {93, 93, 93, 48, 48, 93} ; // Поза сидя. Сдвинуты 4,5 приводы (относительно 93)
unsigned char sit_45_position [6] = {93, 93, 5, 5, 10, 45};
unsigned char train_position [6] = {0, 93, 78, 25, 145, 148}; // координаты : 301, 553,
//unsigned char put_position [6] = {93, 93, 78, 25, 145, 45}; //60, 93, 100, 35, 145, 35
unsigned char put_position [6] = {60, 93, 90, 40, 140, 30};
                            //      60, 93, 100, 35, 145, 35
unsigned char after_put_position [6] = {0, 93, 90, 45, 135, 30};

unsigned char mob_parking_position [4] = {45,90,90,160};//,45,45,49,222
unsigned char mob_ready_position [4] = {45,90,135,165};


//unsigned char mob_pos_10 [4] = {35,90,120,60};
unsigned char mob_pos_11 [4] = {35,90,105,70};
unsigned char mob_pos_12 [4] = {35,90,105,70};

unsigned char mob_pos_13 [4] = {35,90,86,75};
unsigned char mob_pos_14 [4] = {35,90,100,70};
unsigned char mob_pos_15 [4] = {35,90,105,70};
unsigned char mob_pos_16 [4] = {35,90,112,65};
unsigned char mob_pos_17 [4] = {35,90,120,60};
unsigned char mob_pos_18 [4] = {35,90,125,60};
unsigned char mob_pos_19 [4] = {35,90,135,55};
unsigned char mob_pos_20 [4] = {35,90,140,50};
unsigned char mob_pos_21 [4] = {35,90,145,50}; // 21 см. от края кубика до камеры. Центр кубика при этом на 23см.
unsigned char mob_pos_22 [4] = {35,90,168,35};
unsigned char mob_pos_23 [4] = {35,90,172,35}; //35,90,175,32 - тоже подойдет

// moving position 75,90,0,180
//servos=90,90,90,160,45,45,49,222&
//servos=35,90,145,50,45,45,49,222&
//servos=35,90,165,40,45,45,49,222&
//  servos=35,90,168,35,45,45,49,222&

//servos=35,90,86,75,45,45,49,222&


//93, 93, 100, 50, 120, 30
// 60, 93,90, 40, 145, 30

/*
quint8 horse_position [6] = [93, 93, 0, 48, 48, 93];
quint8 horse_mirror_position [6]= [45, 93, 180, 135, 135, 93]  # ;

quint8 ready_to_catch [6] = [93, 93, 2, 15, 110, 93]  # ;
quint8 catch_box [6] = [80, 93, 7, 10, 120, 93]  # ;


//opposite_catch = [93, 93, 180, 125, 40, 60]

quint8 horse_1 [6]= [93, 93, 170, 180, 163, 137]
quint8 horse_2 [6]= [93, 93, 1, 11, 21, 137]
*/

/*
ready_to_catch_2 = [93, 93, 0, 27, 117, 51]
catch_box_2 = [80, 93, 11, 11, 117, 51]
ready_to_put_box_2 = [80, 93, 0, 4, 98, 175]
put_box_2 = [45, 93, 0, 7, 113, 180]
catch_box_3 = [80, 93, 0, 7, 113, 180]
*/
#endif // POSITIONS_H
