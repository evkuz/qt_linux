#ifndef MAINPROCESS_H
#define MAINPROCESS_H

#include <QtCore>
#include <QObject>
#include <QDebug>
#include "jsoninfo.h"

class MainProcess : public QObject
{
    Q_OBJECT
public:
    explicit MainProcess(QObject *parent = nullptr);
    ~MainProcess();

    JsonInfo    *jsnStore;
    QJsonObject mainjsnObj;
    QJsonObject launchActionAnswer;  // Ответ на запуск экшена

    void ProcessAction(int indexOfCommand, QJsonObject &theObj); // Отрабатывает команду
signals:

};

#endif // MAINPROCESS_H
