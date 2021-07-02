#ifndef ROBO_MATH_H
#define ROBO_MATH_H

#include <QObject>
#include <QtMath>

class Robo_Math : public QObject
{
    Q_OBJECT
public:
    explicit Robo_Math(QObject *parent = nullptr);
    ~Robo_Math();

#define L_full 330 // см. полная длина робота
#define L1 105 //L1 от привода 5 до привода 4 - 105 мм если от центра привода и 120, если от пола.
#define L2 87 //L2 от привода 4 до привода 3 -  87 мм
#define L3 115 //L3 от привода 3 до середины хвата -  115 мм
#define z0 77
#define X_base 1200 //x-координата подножия робота
#define Y_base 1240 //y-координата подножия робота

#define pixel2mm 3.375 // ДЛина в 200мм дает разницу в 675 пикселей

// 1190, 356 (area: 22500 px^2), 255, 0, 93, 48, 25, 133, 96
// x0 = 1240(pix), y0 = 1040(pix), z0 = 77(mm)

/*
 L = f(s5) + f(s4) + f(s3)

*/
uint16_t L; // Полная длина робота = расстояние до кубика
uint8_t  S54, S43, S30;

int el1, el2, el3, EL;


float Get_EL(int X, int Y); // Вычисляем длину до кубика в мм по координатам его центра X,Y


signals:
void Return_EL_Signal(float el);
void Return_FW_Kinematic_XYZ_Signal(int X, int Y, int Z, float EL);

public slots:
void Pass_XY_Slot(int x_pix, int y_pix);
void FW_Kinemaic_Slot(int S3, int S4, int S5, int l1, int l2, int l3); // Углы приводов.

};

#endif // ROBO_MATH_H
