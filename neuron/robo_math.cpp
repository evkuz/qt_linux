#include "robo_math.h"

Robo_Math::Robo_Math(QObject *parent) : QObject(parent)
{
  el1 = L1;
  el2 = L2;
  el3 = L3;
}
//++++++++++++++++++++++++++++
Robo_Math::~Robo_Math()
{
    ;
}
//++++++++++++++++++++++++++++
float Robo_Math::Get_EL(int X, int Y)
{
    int x, y;
    float x_mm, y_mm;

    x = X_base - X;
    y = Y_base - Y;

    x_mm = x * pixel2mm;
    y_mm = y * pixel2mm;

    // return qSqrt(x_pix*x_pix + y_pix*y_pix);
    return y_mm;
} // Get_EL
//++++++++++++++++++++++++++++

void Robo_Math::Pass_XY_Slot(int x_pix, int y_pix)
{
    EL = this->Get_EL(x_pix, y_pix);
    EL = qSqrt(EL);
    emit Return_EL_Signal(EL);
}// Pass_XY_Slot
//++++++++++++++++++++++++++++++++++++++
//Длины звеньев передаются в мм.
void Robo_Math::FW_Kinemaic_Slot(int S3, int S4, int S5, int l1, int l2, int l3)
{
    float EL1, dEL1, EL2, dEL2, EL3, dEL3, dEL;
    float x1, y1, x2, y2, x3, y3, z1, z2, z3;
    float fi1, fi2, fi3, alpha1, alpha2, alpha3, beta, psi1, deltaS4;

    deltaS4 = 7.150;
    alpha1 = 180-S5;
    fi1 = 90-alpha1;
    beta = 3.0; // (96-93)
    alpha2 = S4 + 90 + deltaS4;
    alpha3 = S3 + 90;

    EL1 = l1; EL2 = l2; EL3 = l3;

    //Вычитаем, т.к. координаты меняются в другую сторону  - от робота уменьшаются.
    float xBase = X_base / pixel2mm;
    float yBase = Y_base / pixel2mm;


    dEL1 = abs(EL1*qCos(alpha1));
    x1 = xBase - abs(dEL1*qCos(beta));
    y1 = yBase - dEL1*qSin(beta);
    z1 = z0 + EL1*qSin(alpha1);


    dEL2 = abs(EL2*qSin(alpha2 - fi1)); // - fi1
    x2 = x1 - dEL2*qCos(beta);
    y2 = y1 -  dEL2*qSin(beta);
    z2 = z1 + EL2*qCos(alpha2 - fi1);

    psi1 = 90 - (alpha2 - fi1);
    fi2 = psi1 + 90;
    fi3 = alpha2 - fi2;

    dEL3 = abs(EL3*qSin(fi3));
    x3 = x2 - dEL3*qCos(beta);
    y3 = y2 - dEL3*qSin(beta);
    z3 = z2 + EL3*qCos(alpha3 - fi2);


    dEL = dEL1 + dEL2 + dEL3;
    // Получили длину в пикселях, надо преобразовать в мм.
   // dEL /=3.5; // Вот теперь в мм
    //А x,y, надо обратно в пиксели
    emit Return_FW_Kinematic_XYZ_Signal(x3*pixel2mm, y3, z3, dEL);





}
