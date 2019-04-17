/********************************************************************************
** Form generated from reading UI file 'sdrcloud.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SDRCLOUD_H
#define UI_SDRCLOUD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SDRCloudClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SDRCloudClass)
    {
        if (SDRCloudClass->objectName().isEmpty())
            SDRCloudClass->setObjectName(QStringLiteral("SDRCloudClass"));
        SDRCloudClass->resize(995, 661);
        centralWidget = new QWidget(SDRCloudClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SDRCloudClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SDRCloudClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 995, 26));
        SDRCloudClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SDRCloudClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SDRCloudClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SDRCloudClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SDRCloudClass->setStatusBar(statusBar);

        retranslateUi(SDRCloudClass);

        QMetaObject::connectSlotsByName(SDRCloudClass);
    } // setupUi

    void retranslateUi(QMainWindow *SDRCloudClass)
    {
        SDRCloudClass->setWindowTitle(QApplication::translate("SDRCloudClass", "SDRCloud", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SDRCloudClass: public Ui_SDRCloudClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SDRCLOUD_H
