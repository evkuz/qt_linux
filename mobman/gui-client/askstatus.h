#ifndef ASKSTATUS_H
#define ASKSTATUS_H

#include <QObject>
#include <QTimer>

class askStatus : public QObject
{
    Q_OBJECT
public:
    QTimer *timer;
    explicit askStatus(QObject *parent = nullptr);

signals:

};

#endif // ASKSTATUS_H
