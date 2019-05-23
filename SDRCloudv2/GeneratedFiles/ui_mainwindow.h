/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actDebug;
    QAction *actOpenRTL;
    QAction *actCloseRTL;
    QAction *actOpenImgWindow;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuRTL;
    QMenu *menuImage;
    QMenu *menuSate;
    QMenu *menuConfig;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(600, 400);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/Resources/sdrcloud.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindowClass->setWindowIcon(icon);
        MainWindowClass->setIconSize(QSize(256, 256));
        actDebug = new QAction(MainWindowClass);
        actDebug->setObjectName(QStringLiteral("actDebug"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon/Resources/ASX_Cancel_blue_16x.png"), QSize(), QIcon::Normal, QIcon::Off);
        actDebug->setIcon(icon1);
        actOpenRTL = new QAction(MainWindowClass);
        actOpenRTL->setObjectName(QStringLiteral("actOpenRTL"));
        actCloseRTL = new QAction(MainWindowClass);
        actCloseRTL->setObjectName(QStringLiteral("actCloseRTL"));
        actCloseRTL->setEnabled(false);
        actOpenImgWindow = new QAction(MainWindowClass);
        actOpenImgWindow->setObjectName(QStringLiteral("actOpenImgWindow"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRTL = new QMenu(menuBar);
        menuRTL->setObjectName(QStringLiteral("menuRTL"));
        menuImage = new QMenu(menuBar);
        menuImage->setObjectName(QStringLiteral("menuImage"));
        menuSate = new QMenu(menuBar);
        menuSate->setObjectName(QStringLiteral("menuSate"));
        menuConfig = new QMenu(menuBar);
        menuConfig->setObjectName(QStringLiteral("menuConfig"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuRTL->menuAction());
        menuBar->addAction(menuImage->menuAction());
        menuBar->addAction(menuSate->menuAction());
        menuBar->addAction(menuConfig->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actDebug);
        menuRTL->addAction(actOpenRTL);
        menuRTL->addAction(actCloseRTL);
        menuRTL->addAction(actDebug);
        menuImage->addAction(actOpenImgWindow);
        menuImage->addAction(actDebug);
        menuSate->addAction(actDebug);
        menuConfig->addAction(actDebug);
        menuHelp->addAction(actDebug);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", Q_NULLPTR));
        actDebug->setText(QApplication::translate("MainWindowClass", "debug", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actDebug->setToolTip(QApplication::translate("MainWindowClass", "\344\273\205\347\224\250\344\272\216\346\265\213\350\257\225", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actOpenRTL->setText(QApplication::translate("MainWindowClass", "\346\211\223\345\274\200RTL\350\256\276\345\244\207...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actOpenRTL->setToolTip(QApplication::translate("MainWindowClass", "\346\211\223\345\274\200\344\270\200\344\270\252RTL\350\256\276\345\244\207", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actCloseRTL->setText(QApplication::translate("MainWindowClass", "\345\205\263\351\227\255RTL\350\256\276\345\244\207", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actCloseRTL->setToolTip(QApplication::translate("MainWindowClass", "\345\205\263\351\227\255\346\211\223\345\274\200\347\232\204RTL\350\256\276\345\244\207", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actOpenImgWindow->setText(QApplication::translate("MainWindowClass", "\346\211\223\345\274\200\345\233\276\345\203\217\347\252\227\345\217\243...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actOpenImgWindow->setToolTip(QApplication::translate("MainWindowClass", "\346\211\223\345\274\200\345\233\276\345\203\217\347\252\227\345\217\243", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        menuFile->setTitle(QApplication::translate("MainWindowClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menuRTL->setTitle(QApplication::translate("MainWindowClass", "RTL", Q_NULLPTR));
        menuImage->setTitle(QApplication::translate("MainWindowClass", "\345\233\276\345\203\217", Q_NULLPTR));
        menuSate->setTitle(QApplication::translate("MainWindowClass", "\345\215\253\346\230\237", Q_NULLPTR));
        menuConfig->setTitle(QApplication::translate("MainWindowClass", "\350\256\276\347\275\256", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindowClass", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
