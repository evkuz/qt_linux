#ifndef ROBOMATH_H
#define ROBOMATH_H

#include <QObject>

class RoboMath : public QObject
{
    Q_OBJECT
public:
    explicit RoboMath(QObject *parent = nullptr);
    ~RoboMath();
#define L1 30 // Длина 1-е звено в см.
#define L1 30 // Длина 2-е звено в см.
    int calc_len (uint8_t fi5, uint_fast8_t fi4, uint8_t fi3);

signals:

};

#endif // ROBOMATH_H
