#ifndef CVDEVICE_H
#define CVDEVICE_H

#include <QObject>

class CVDevice : public QObject
{
    Q_OBJECT
public:
    explicit CVDevice(QObject *parent = nullptr);

signals:

};

#endif // CVDEVICE_H
