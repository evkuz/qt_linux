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
#include <QtWidgets/QSpinBox>
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
    QPushButton *set_posButton;
    QPushButton *stand_upButton;
    QLabel *S1_label;
    QLabel *S2_label;
    QLabel *S3_label;
    QLabel *S4_label;
    QLabel *S5_label;
    QLabel *S6_label;
    QPushButton *sitButton;
    QPushButton *getXYButton;
    QPushButton *socketButton;
    QSpinBox *servo_1_spinBox;
    QSpinBox *servo_2_spinBox;
    QSpinBox *servo_3_spinBox;
    QSpinBox *servo_4_spinBox;
    QSpinBox *servo_5_spinBox;
    QSpinBox *servo_6_spinBox;
    QLineEdit *All_Servos_lineEdit;
    QPushButton *submitButton;
    QPushButton *trainButton;
    QLabel *threadlabel;
    QPushButton *getBackButton;
    QPushButton *fixButton;
    QPushButton *PUTButton;
    QPushButton *GetBackFromServoButton;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(950, 482);
        layoutWidget = new QWidget(MainWindow);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(200, 40, 278, 29));
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
        clampButton->setGeometry(QRect(370, 160, 89, 51));
        set_posButton = new QPushButton(MainWindow);
        set_posButton->setObjectName(QString::fromUtf8("set_posButton"));
        set_posButton->setGeometry(QRect(20, 150, 341, 61));
        stand_upButton = new QPushButton(MainWindow);
        stand_upButton->setObjectName(QString::fromUtf8("stand_upButton"));
        stand_upButton->setGeometry(QRect(510, 70, 141, 61));
        S1_label = new QLabel(MainWindow);
        S1_label->setObjectName(QString::fromUtf8("S1_label"));
        S1_label->setGeometry(QRect(40, 240, 61, 17));
        S2_label = new QLabel(MainWindow);
        S2_label->setObjectName(QString::fromUtf8("S2_label"));
        S2_label->setGeometry(QRect(190, 240, 61, 17));
        S3_label = new QLabel(MainWindow);
        S3_label->setObjectName(QString::fromUtf8("S3_label"));
        S3_label->setGeometry(QRect(360, 240, 61, 17));
        S4_label = new QLabel(MainWindow);
        S4_label->setObjectName(QString::fromUtf8("S4_label"));
        S4_label->setGeometry(QRect(530, 240, 181, 17));
        S5_label = new QLabel(MainWindow);
        S5_label->setObjectName(QString::fromUtf8("S5_label"));
        S5_label->setGeometry(QRect(680, 240, 61, 17));
        S6_label = new QLabel(MainWindow);
        S6_label->setObjectName(QString::fromUtf8("S6_label"));
        S6_label->setGeometry(QRect(840, 240, 61, 17));
        sitButton = new QPushButton(MainWindow);
        sitButton->setObjectName(QString::fromUtf8("sitButton"));
        sitButton->setGeometry(QRect(20, 80, 131, 51));
        getXYButton = new QPushButton(MainWindow);
        getXYButton->setObjectName(QString::fromUtf8("getXYButton"));
        getXYButton->setGeometry(QRect(510, 150, 151, 25));
        socketButton = new QPushButton(MainWindow);
        socketButton->setObjectName(QString::fromUtf8("socketButton"));
        socketButton->setGeometry(QRect(750, 110, 151, 71));
        servo_1_spinBox = new QSpinBox(MainWindow);
        servo_1_spinBox->setObjectName(QString::fromUtf8("servo_1_spinBox"));
        servo_1_spinBox->setGeometry(QRect(10, 270, 121, 61));
        QFont font;
        font.setPointSize(24);
        servo_1_spinBox->setFont(font);
        servo_1_spinBox->setStyleSheet(QString::fromUtf8("QSpinBox::down-button{\n"
"    width: 50\n"
"}\n"
"QSpinBox::up-button{\n"
"    width: 50\n"
"}\n"
""));
        servo_1_spinBox->setMaximum(180);
        servo_1_spinBox->setValue(90);
        servo_2_spinBox = new QSpinBox(MainWindow);
        servo_2_spinBox->setObjectName(QString::fromUtf8("servo_2_spinBox"));
        servo_2_spinBox->setGeometry(QRect(170, 270, 121, 61));
        servo_2_spinBox->setFont(font);
        servo_2_spinBox->setStyleSheet(QString::fromUtf8("QSpinBox::down-button{\n"
"    width: 50\n"
"}\n"
"QSpinBox::up-button{\n"
"    width: 50\n"
"}\n"
""));
        servo_2_spinBox->setMaximum(180);
        servo_2_spinBox->setValue(90);
        servo_3_spinBox = new QSpinBox(MainWindow);
        servo_3_spinBox->setObjectName(QString::fromUtf8("servo_3_spinBox"));
        servo_3_spinBox->setGeometry(QRect(330, 270, 121, 61));
        servo_3_spinBox->setFont(font);
        servo_3_spinBox->setStyleSheet(QString::fromUtf8("QSpinBox::down-button{\n"
"    width: 50\n"
"}\n"
"QSpinBox::up-button{\n"
"    width: 50\n"
"}\n"
""));
        servo_3_spinBox->setMaximum(180);
        servo_3_spinBox->setValue(90);
        servo_4_spinBox = new QSpinBox(MainWindow);
        servo_4_spinBox->setObjectName(QString::fromUtf8("servo_4_spinBox"));
        servo_4_spinBox->setGeometry(QRect(490, 270, 121, 61));
        servo_4_spinBox->setFont(font);
        servo_4_spinBox->setStyleSheet(QString::fromUtf8("QSpinBox::down-button{\n"
"    width: 50\n"
"}\n"
"QSpinBox::up-button{\n"
"    width: 50\n"
"}\n"
""));
        servo_4_spinBox->setMaximum(180);
        servo_4_spinBox->setValue(90);
        servo_5_spinBox = new QSpinBox(MainWindow);
        servo_5_spinBox->setObjectName(QString::fromUtf8("servo_5_spinBox"));
        servo_5_spinBox->setGeometry(QRect(650, 270, 121, 61));
        servo_5_spinBox->setFont(font);
        servo_5_spinBox->setStyleSheet(QString::fromUtf8("QSpinBox::down-button{\n"
"    width: 50\n"
"}\n"
"QSpinBox::up-button{\n"
"    width: 50\n"
"}\n"
""));
        servo_5_spinBox->setMaximum(180);
        servo_5_spinBox->setValue(90);
        servo_6_spinBox = new QSpinBox(MainWindow);
        servo_6_spinBox->setObjectName(QString::fromUtf8("servo_6_spinBox"));
        servo_6_spinBox->setGeometry(QRect(810, 270, 121, 61));
        servo_6_spinBox->setFont(font);
        servo_6_spinBox->setStyleSheet(QString::fromUtf8("QSpinBox::down-button{\n"
"    width: 50\n"
"}\n"
"QSpinBox::up-button{\n"
"    width: 50\n"
"}\n"
""));
        servo_6_spinBox->setMaximum(180);
        servo_6_spinBox->setValue(90);
        All_Servos_lineEdit = new QLineEdit(MainWindow);
        All_Servos_lineEdit->setObjectName(QString::fromUtf8("All_Servos_lineEdit"));
        All_Servos_lineEdit->setGeometry(QRect(10, 390, 441, 51));
        All_Servos_lineEdit->setFont(font);
        All_Servos_lineEdit->setStyleSheet(QString::fromUtf8(""));
        All_Servos_lineEdit->setMaxLength(28);
        submitButton = new QPushButton(MainWindow);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setGeometry(QRect(460, 390, 121, 51));
        QFont font1;
        font1.setPointSize(18);
        submitButton->setFont(font1);
        submitButton->setStyleSheet(QString::fromUtf8(""));
        trainButton = new QPushButton(MainWindow);
        trainButton->setObjectName(QString::fromUtf8("trainButton"));
        trainButton->setGeometry(QRect(730, 380, 211, 91));
        QFont font2;
        font2.setPointSize(18);
        font2.setItalic(false);
        trainButton->setFont(font2);
        threadlabel = new QLabel(MainWindow);
        threadlabel->setObjectName(QString::fromUtf8("threadlabel"));
        threadlabel->setGeometry(QRect(20, 20, 91, 31));
        getBackButton = new QPushButton(MainWindow);
        getBackButton->setObjectName(QString::fromUtf8("getBackButton"));
        getBackButton->setGeometry(QRect(190, 80, 261, 51));
        fixButton = new QPushButton(MainWindow);
        fixButton->setObjectName(QString::fromUtf8("fixButton"));
        fixButton->setGeometry(QRect(700, 0, 211, 91));
        QFont font3;
        font3.setPointSize(18);
        font3.setItalic(true);
        fixButton->setFont(font3);
        PUTButton = new QPushButton(MainWindow);
        PUTButton->setObjectName(QString::fromUtf8("PUTButton"));
        PUTButton->setGeometry(QRect(50, 10, 99, 51));
        PUTButton->setFont(font3);
        GetBackFromServoButton = new QPushButton(MainWindow);
        GetBackFromServoButton->setObjectName(QString::fromUtf8("GetBackFromServoButton"));
        GetBackFromServoButton->setGeometry(QRect(590, 390, 131, 61));

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "main", nullptr));
        openButton->setText(QCoreApplication::translate("MainWindow", "OPEN", nullptr));
        closeButton->setText(QCoreApplication::translate("MainWindow", "CLOSE", nullptr));
        clampButton->setText(QCoreApplication::translate("MainWindow", "CLAMP", nullptr));
        set_posButton->setText(QCoreApplication::translate("MainWindow", "SET POSITION", nullptr));
        stand_upButton->setText(QCoreApplication::translate("MainWindow", "Stand UP", nullptr));
        S1_label->setText(QCoreApplication::translate("MainWindow", "Servo 1", nullptr));
        S2_label->setText(QCoreApplication::translate("MainWindow", "Servo 2", nullptr));
        S3_label->setText(QCoreApplication::translate("MainWindow", "Servo 3", nullptr));
        S4_label->setText(QCoreApplication::translate("MainWindow", "Servo 4", nullptr));
        S5_label->setText(QCoreApplication::translate("MainWindow", "Servo 5", nullptr));
        S6_label->setText(QCoreApplication::translate("MainWindow", "Servo 6", nullptr));
        sitButton->setText(QCoreApplication::translate("MainWindow", "SIT", nullptr));
        getXYButton->setText(QCoreApplication::translate("MainWindow", "Get XY", nullptr));
        socketButton->setText(QCoreApplication::translate("MainWindow", "Read Detetor", nullptr));
        submitButton->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        trainButton->setText(QCoreApplication::translate("MainWindow", "TRAIN", nullptr));
        threadlabel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        getBackButton->setText(QCoreApplication::translate("MainWindow", "GetBack", nullptr));
        fixButton->setText(QCoreApplication::translate("MainWindow", "Fix Data", nullptr));
        PUTButton->setText(QCoreApplication::translate("MainWindow", "PUT", nullptr));
        GetBackFromServoButton->setText(QCoreApplication::translate("MainWindow", "BACK FROM SpB", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
