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

    bool finthread;   //Флаг работы самого потока. Поток закрываем по нажатию кнопок в окне пользователя
    bool pause_thread; // True - значит ставим на паузу


public slots:

void A_SLOT();

signals:
    void SendToTcp_Signal();
    void finished();   /* Сигнал о завершении работы Потока. */
};

#endif // MYTHREAD_H
