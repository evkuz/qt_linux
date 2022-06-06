#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
class myThread : public QObject
{
    Q_OBJECT
public:
//    explicit myThread(QObject *parent = nullptr);
    myThread();
    ~myThread();

public slots:

void A_SLOT();

signals:
    void SendToTcp_Signal();
};

#endif // MYTHREAD_H
