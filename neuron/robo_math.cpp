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
void Robo_Math::FW_Kinemaic_Slot(int S3, int S4, int S5, int l1, int l2, int l3)
{
    float EL1, dEL1, EL2, dEL2, EL3, dEL3;
    float x1, y1, x2, y2, x3, y3, z1, z2, z3;
    float fi1, fi2, fi3, alpha1, alpha2, alpha3, beta, psi1;

    alpha1 = 180-S5;
    fi1 = 90-alpha1;
    beta = 3.0; // (96-93)
    alpha2 = S4;
    alpha3 = S3;

    EL1 = l1; EL2 = l2; EL3 = l3;

    dEL1 = EL1*qCos(alpha1);
    x1 = dEL1*qCos(beta);
    y1 = dEL1*qSin(beta);
    z1 = EL1*qSin(alpha1);


    dEL2 = EL2*qSin(alpha2 - fi1);
    x2 = x1 + dEL2*qCos(beta);
    y2 = y1 + dEL2*qSin(beta);
    z2 = z1 + EL2*qCos(alpha2 - fi1);

    psi1 = 90 - (alpha2 - fi1);
    fi2 = psi1 + 90;
    fi3 = alpha2 - fi2;
    dEL3 = z2 + EL3*qSin(fi3);
    x3 = x2 + dEL3*qCos(beta);
    y3 = y2 + dEL3*qSin(beta);
    z3 = z2 + EL3*qCos(alpha3 - fi2);



    emit Return_FW_Kinematic_XYZ_Signal(x3, y3, z3);





}
