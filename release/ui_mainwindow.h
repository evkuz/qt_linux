/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QComboBox *comL;
    QPushButton *openButton;
    QPushButton *closeButton;
    QPushButton *clampButton;
    QLineEdit *servo_1_lineEdit;
    QLineEdit *servo_2_lineEdit;
    QLineEdit *servo_3_lineEdit;
    QLineEdit *servo_4_lineEdit;
    QLineEdit *servo_5_lineEdit;
    QLineEdit *servo_6_lineEdit;
    QPushButton *set_posButton;
    QPushButton *stand_upButton;
    QLabel *S1_label;
    QLabel *S2_label;
    QLabel *S3_label;
    QLabel *S4_label;
    QLabel *S5_label;
    QLabel *S6_label;
    QPushButton *sitButton;
    QPushButton *getBoxButton;
    QSlider *S6_verSlider;
    QSlider *S5_verSlider;
    QSlider *S4_verSlider;
    QSlider *S3_verSlider;
    QSlider *S2_verSlider;
    QSlider *S1_verSlider;
    QPushButton *horse_1_Button;
    QPushButton *horse_2_Button;
    QPushButton *put_getBoxButton;
    QPushButton *combSitStandButton;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 591);
        layoutWidget = new QWidget(MainWindow);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(200, 40, 278, 27));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        comL = new QComboBox(layoutWidget);
        comL->setObjectName(QString::fromUtf8("comL"));

        horizontalLayout->addWidget(comL);

        openButton = new QPushButton(layoutWidget);
        openButton->setObjectName(QString::fromUtf8("openButton"));

        horizontalLayout->addWidget(openButton);

        closeButton = new QPushButton(layoutWidget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        horizontalLayout->addWidget(closeButton);

        clampButton = new QPushButton(MainWindow);
        clampButton->setObjectName(QString::fromUtf8("clampButton"));
        clampButton->setGeometry(QRect(450, 110, 89, 25));
        servo_1_lineEdit = new QLineEdit(MainWindow);
        servo_1_lineEdit->setObjectName(QString::fromUtf8("servo_1_lineEdit"));
        servo_1_lineEdit->setGeometry(QRect(40, 250, 61, 25));
        servo_2_lineEdit = new QLineEdit(MainWindow);
        servo_2_lineEdit->setObjectName(QString::fromUtf8("servo_2_lineEdit"));
        servo_2_lineEdit->setGeometry(QRect(120, 250, 61, 25));
        servo_3_lineEdit = new QLineEdit(MainWindow);
        servo_3_lineEdit->setObjectName(QString::fromUtf8("servo_3_lineEdit"));
        servo_3_lineEdit->setGeometry(QRect(200, 250, 61, 25));
        servo_4_lineEdit = new QLineEdit(MainWindow);
        servo_4_lineEdit->setObjectName(QString::fromUtf8("servo_4_lineEdit"));
        servo_4_lineEdit->setGeometry(QRect(280, 250, 61, 25));
        servo_5_lineEdit = new QLineEdit(MainWindow);
        servo_5_lineEdit->setObjectName(QString::fromUtf8("servo_5_lineEdit"));
        servo_5_lineEdit->setGeometry(QRect(360, 250, 61, 25));
        servo_6_lineEdit = new QLineEdit(MainWindow);
        servo_6_lineEdit->setObjectName(QString::fromUtf8("servo_6_lineEdit"));
        servo_6_lineEdit->setGeometry(QRect(440, 250, 61, 25));
        set_posButton = new QPushButton(MainWindow);
        set_posButton->setObjectName(QString::fromUtf8("set_posButton"));
        set_posButton->setGeometry(QRect(140, 180, 341, 25));
        stand_upButton = new QPushButton(MainWindow);
        stand_upButton->setObjectName(QString::fromUtf8("stand_upButton"));
        stand_upButton->setGeometry(QRect(230, 110, 89, 25));
        S1_label = new QLabel(MainWindow);
        S1_label->setObjectName(QString::fromUtf8("S1_label"));
        S1_label->setGeometry(QRect(40, 230, 61, 17));
        S2_label = new QLabel(MainWindow);
        S2_label->setObjectName(QString::fromUtf8("S2_label"));
        S2_label->setGeometry(QRect(120, 230, 61, 17));
        S3_label = new QLabel(MainWindow);
        S3_label->setObjectName(QString::fromUtf8("S3_label"));
        S3_label->setGeometry(QRect(200, 230, 61, 17));
        S4_label = new QLabel(MainWindow);
        S4_label->setObjectName(QString::fromUtf8("S4_label"));
        S4_label->setGeometry(QRect(280, 230, 61, 17));
        S5_label = new QLabel(MainWindow);
        S5_label->setObjectName(QString::fromUtf8("S5_label"));
        S5_label->setGeometry(QRect(360, 230, 61, 17));
        S6_label = new QLabel(MainWindow);
        S6_label->setObjectName(QString::fromUtf8("S6_label"));
        S6_label->setGeometry(QRect(440, 230, 61, 17));
        sitButton = new QPushButton(MainWindow);
        sitButton->setObjectName(QString::fromUtf8("sitButton"));
        sitButton->setGeometry(QRect(120, 110, 89, 25));
        getBoxButton = new QPushButton(MainWindow);
        getBoxButton->setObjectName(QString::fromUtf8("getBoxButton"));
        getBoxButton->setGeometry(QRect(340, 110, 89, 25));
        S6_verSlider = new QSlider(MainWindow);
        S6_verSlider->setObjectName(QString::fromUtf8("S6_verSlider"));
        S6_verSlider->setGeometry(QRect(460, 290, 16, 251));
        S6_verSlider->setMaximum(180);
        S6_verSlider->setValue(93);
        S6_verSlider->setOrientation(Qt::Vertical);
        S5_verSlider = new QSlider(MainWindow);
        S5_verSlider->setObjectName(QString::fromUtf8("S5_verSlider"));
        S5_verSlider->setGeometry(QRect(380, 290, 16, 251));
        S5_verSlider->setMaximum(180);
        S5_verSlider->setValue(93);
        S5_verSlider->setOrientation(Qt::Vertical);
        S4_verSlider = new QSlider(MainWindow);
        S4_verSlider->setObjectName(QString::fromUtf8("S4_verSlider"));
        S4_verSlider->setGeometry(QRect(300, 290, 16, 251));
        S4_verSlider->setMaximum(180);
        S4_verSlider->setValue(93);
        S4_verSlider->setOrientation(Qt::Vertical);
        S3_verSlider = new QSlider(MainWindow);
        S3_verSlider->setObjectName(QString::fromUtf8("S3_verSlider"));
        S3_verSlider->setGeometry(QRect(220, 290, 16, 251));
        S3_verSlider->setMaximum(180);
        S3_verSlider->setValue(93);
        S3_verSlider->setOrientation(Qt::Vertical);
        S2_verSlider = new QSlider(MainWindow);
        S2_verSlider->setObjectName(QString::fromUtf8("S2_verSlider"));
        S2_verSlider->setGeometry(QRect(140, 290, 16, 251));
        S2_verSlider->setMaximum(180);
        S2_verSlider->setValue(93);
        S2_verSlider->setOrientation(Qt::Vertical);
        S1_verSlider = new QSlider(MainWindow);
        S1_verSlider->setObjectName(QString::fromUtf8("S1_verSlider"));
        S1_verSlider->setGeometry(QRect(60, 290, 16, 251));
        S1_verSlider->setMaximum(180);
        S1_verSlider->setValue(93);
        S1_verSlider->setOrientation(Qt::Vertical);
        horse_1_Button = new QPushButton(MainWindow);
        horse_1_Button->setObjectName(QString::fromUtf8("horse_1_Button"));
        horse_1_Button->setGeometry(QRect(610, 110, 89, 25));
        horse_2_Button = new QPushButton(MainWindow);
        horse_2_Button->setObjectName(QString::fromUtf8("horse_2_Button"));
        horse_2_Button->setGeometry(QRect(610, 160, 89, 25));
        put_getBoxButton = new QPushButton(MainWindow);
        put_getBoxButton->setObjectName(QString::fromUtf8("put_getBoxButton"));
        put_getBoxButton->setGeometry(QRect(600, 250, 141, 25));
        combSitStandButton = new QPushButton(MainWindow);
        combSitStandButton->setObjectName(QString::fromUtf8("combSitStandButton"));
        combSitStandButton->setGeometry(QRect(600, 320, 131, 25));

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "main", nullptr));
        openButton->setText(QCoreApplication::translate("MainWindow", "OPEN", nullptr));
        closeButton->setText(QCoreApplication::translate("MainWindow", "CLOSE", nullptr));
        clampButton->setText(QCoreApplication::translate("MainWindow", "CLAMP", nullptr));
        servo_1_lineEdit->setText(QCoreApplication::translate("MainWindow", "93", nullptr));
        servo_2_lineEdit->setText(QCoreApplication::translate("MainWindow", "93", nullptr));
        servo_3_lineEdit->setText(QCoreApplication::translate("MainWindow", "93", nullptr));
        servo_4_lineEdit->setText(QCoreApplication::translate("MainWindow", "93", nullptr));
        servo_5_lineEdit->setText(QCoreApplication::translate("MainWindow", "93", nullptr));
        servo_6_lineEdit->setText(QCoreApplication::translate("MainWindow", "93", nullptr));
        set_posButton->setText(QCoreApplication::translate("MainWindow", "SET POSITION", nullptr));
        stand_upButton->setText(QCoreApplication::translate("MainWindow", "Stand UP", nullptr));
        S1_label->setText(QCoreApplication::translate("MainWindow", "Servo 1", nullptr));
        S2_label->setText(QCoreApplication::translate("MainWindow", "Servo 2", nullptr));
        S3_label->setText(QCoreApplication::translate("MainWindow", "Servo 3", nullptr));
        S4_label->setText(QCoreApplication::translate("MainWindow", "Servo 4", nullptr));
        S5_label->setText(QCoreApplication::translate("MainWindow", "Servo 5", nullptr));
        S6_label->setText(QCoreApplication::translate("MainWindow", "Servo 6", nullptr));
        sitButton->setText(QCoreApplication::translate("MainWindow", "SIT", nullptr));
        getBoxButton->setText(QCoreApplication::translate("MainWindow", "Get Box", nullptr));
        horse_1_Button->setText(QCoreApplication::translate("MainWindow", "Horse-1", nullptr));
        horse_2_Button->setText(QCoreApplication::translate("MainWindow", "Horse-2", nullptr));
        put_getBoxButton->setText(QCoreApplication::translate("MainWindow", "PUT + GET Box", nullptr));
        combSitStandButton->setText(QCoreApplication::translate("MainWindow", "SIT + Stand UP", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
