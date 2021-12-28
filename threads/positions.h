#ifndef POSITIONS_H
#define POSITIONS_H
/*Координаты начальной/стартовой позиции*/
#include <QtCore>

const char hwr_Start_position [6] = {93, 93, 93, 93, 93, 93}; // servo1,,,servo6
const char sit_down_position [6] = {93, 93, 93, 48, 48, 93} ; // Поза сидя. Сдвинуты 4,5 приводы (относительно 93)
const char sit_45_position [6] = {93, 93, 5, 5, 10, 45};
unsigned char train_position [6] = {0, 93, 78, 25, 145, 148}; // координаты : 301, 553,
//unsigned char put_position [6] = {93, 93, 78, 25, 145, 45}; //60, 93, 100, 35, 145, 35
unsigned char put_position [6] = {60, 93, 90, 40, 140, 30};
                            //      60, 93, 100, 35, 145, 35
unsigned char after_put_position [6] = {0, 93, 90, 45, 135, 30};

unsigned char nw_1st_position[6] = {75, 93, 10, 45, 65, 179};
unsigned char nw_2nd_position[6] = {75, 93, 10, 45, 65, 20};
unsigned char nw_3rd_position[6] = {75, 93, 20, 45, 85, 20};

//+++++++++++++++++++++ Позиции для передвижения ёлки

unsigned char mv_01_tree_position[6] = {0, 93, 30, 70, 100, 170};
unsigned char mv_02_tree_position[6] = {120, 93, 70, 70, 105, 165};
unsigned char mv_03_tree_position[6] = {120, 93, 70, 70, 105, 20};


unsigned char mv_04_tree_position[6] = {0, 93, 70, 70, 135, 20};
unsigned char mv_05_tree_position[6] = {0, 93, 70, 100, 135, 20};

//++++++++++++++++++++++++++++  Снеговик
unsigned char olaf_01[6]={0, 93, 45, 45, 93, 165};
unsigned char olaf_02[6]={0, 93, 15, 45, 93, 165};
unsigned char olaf_03[6]={0, 93, 25, 45, 93, 165};
unsigned char olaf_04[6]={63, 93, 25, 45, 93, 165};
unsigned char olaf_05[6]={63, 93, 65, 45, 93, 165};
unsigned char olaf_06[6]={63, 93, 65, 45, 135, 40};






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
