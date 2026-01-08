/********************************************************************************
** Form generated from reading UI file 'Widget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *mainLayout;
    QWidget *mainPlotContainer;
    QVBoxLayout *verticalLayout_4;
    QCustomPlot *customplot;
    QWidget *controlPanel;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_SerialConfig;
    QSpacerItem *horizontalSpacer;
    QLabel *label_SerialPort;
    QComboBox *cbPort;
    QLabel *label_BaudRate;
    QComboBox *cbBaud;
    QPushButton *btnSwitchPort;
    QPushButton *btnTest;
    QScrollArea *scrollArea;
    QWidget *subContainer;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(627, 483);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setObjectName("verticalLayout");
        mainLayout = new QHBoxLayout();
        mainLayout->setObjectName("mainLayout");
        mainPlotContainer = new QWidget(Widget);
        mainPlotContainer->setObjectName("mainPlotContainer");
        verticalLayout_4 = new QVBoxLayout(mainPlotContainer);
        verticalLayout_4->setObjectName("verticalLayout_4");
        customplot = new QCustomPlot(mainPlotContainer);
        customplot->setObjectName("customplot");

        verticalLayout_4->addWidget(customplot);


        mainLayout->addWidget(mainPlotContainer);

        controlPanel = new QWidget(Widget);
        controlPanel->setObjectName("controlPanel");
        verticalLayout_3 = new QVBoxLayout(controlPanel);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_SerialConfig = new QLabel(controlPanel);
        label_SerialConfig->setObjectName("label_SerialConfig");

        gridLayout->addWidget(label_SerialConfig, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        label_SerialPort = new QLabel(controlPanel);
        label_SerialPort->setObjectName("label_SerialPort");

        gridLayout->addWidget(label_SerialPort, 1, 0, 1, 1);

        cbPort = new QComboBox(controlPanel);
        cbPort->setObjectName("cbPort");

        gridLayout->addWidget(cbPort, 1, 1, 1, 1);

        label_BaudRate = new QLabel(controlPanel);
        label_BaudRate->setObjectName("label_BaudRate");

        gridLayout->addWidget(label_BaudRate, 2, 0, 1, 1);

        cbBaud = new QComboBox(controlPanel);
        cbBaud->setObjectName("cbBaud");

        gridLayout->addWidget(cbBaud, 2, 1, 1, 1);

        btnSwitchPort = new QPushButton(controlPanel);
        btnSwitchPort->setObjectName("btnSwitchPort");

        gridLayout->addWidget(btnSwitchPort, 3, 0, 1, 1);

        btnTest = new QPushButton(controlPanel);
        btnTest->setObjectName("btnTest");

        gridLayout->addWidget(btnTest, 3, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        scrollArea = new QScrollArea(controlPanel);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        subContainer = new QWidget();
        subContainer->setObjectName("subContainer");
        subContainer->setGeometry(QRect(0, 0, 159, 314));
        scrollArea->setWidget(subContainer);

        verticalLayout_2->addWidget(scrollArea);


        verticalLayout_3->addLayout(verticalLayout_2);


        mainLayout->addWidget(controlPanel);

        mainLayout->setStretch(0, 3);
        mainLayout->setStretch(1, 1);

        verticalLayout->addLayout(mainLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label_SerialConfig->setText(QCoreApplication::translate("Widget", "\344\270\262\345\217\243\351\205\215\347\275\256", nullptr));
        label_SerialPort->setText(QCoreApplication::translate("Widget", "\344\270\262\345\217\243\345\217\267", nullptr));
        label_BaudRate->setText(QCoreApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207", nullptr));
        btnSwitchPort->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        btnTest->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
