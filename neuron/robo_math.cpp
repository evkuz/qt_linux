#include "robo_math.h"

Robo_Math::Robo_Math(QObject *parent) : QObject(parent)
{

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
}//++++++++++++++++++++++++++++

void Robo_Math::Pass_XY_Slot(int x_pix, int y_pix)
{
    EL = this->Get_EL(x_pix, y_pix);
    EL = qSqrt(EL);
    emit Return_EL_Signal(EL);
}
