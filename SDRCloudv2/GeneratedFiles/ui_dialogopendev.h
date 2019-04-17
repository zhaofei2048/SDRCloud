/********************************************************************************
** Form generated from reading UI file 'dialogopendev.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGOPENDEV_H
#define UI_DIALOGOPENDEV_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogOpenDev
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *labHint;
    QListWidget *listWidgetDevice;
    QVBoxLayout *verticalLayout;
    QToolButton *btnRefresh;
    QPushButton *btnOK;
    QPushButton *btnCancel;

    void setupUi(QDialog *DialogOpenDev)
    {
        if (DialogOpenDev->objectName().isEmpty())
            DialogOpenDev->setObjectName(QStringLiteral("DialogOpenDev"));
        DialogOpenDev->resize(500, 250);
        DialogOpenDev->setMinimumSize(QSize(500, 250));
        DialogOpenDev->setMaximumSize(QSize(500, 250));
        DialogOpenDev->setSizeGripEnabled(true);
        layoutWidget = new QWidget(DialogOpenDev);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 480, 218));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        labHint = new QLabel(layoutWidget);
        labHint->setObjectName(QStringLiteral("labHint"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        labHint->setFont(font);

        verticalLayout_2->addWidget(labHint);

        listWidgetDevice = new QListWidget(layoutWidget);
        listWidgetDevice->setObjectName(QStringLiteral("listWidgetDevice"));

        verticalLayout_2->addWidget(listWidgetDevice);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        btnRefresh = new QToolButton(layoutWidget);
        btnRefresh->setObjectName(QStringLiteral("btnRefresh"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/Resources/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
        btnRefresh->setIcon(icon);

        verticalLayout->addWidget(btnRefresh);

        btnOK = new QPushButton(layoutWidget);
        btnOK->setObjectName(QStringLiteral("btnOK"));

        verticalLayout->addWidget(btnOK);

        btnCancel = new QPushButton(layoutWidget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

        verticalLayout->addWidget(btnCancel);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(DialogOpenDev);
        QObject::connect(btnOK, SIGNAL(clicked()), DialogOpenDev, SLOT(accept()));
        QObject::connect(btnCancel, SIGNAL(clicked()), DialogOpenDev, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogOpenDev);
    } // setupUi

    void retranslateUi(QDialog *DialogOpenDev)
    {
        DialogOpenDev->setWindowTitle(QApplication::translate("DialogOpenDev", "\346\211\223\345\274\200RTL\350\256\276\345\244\207", Q_NULLPTR));
        labHint->setText(QApplication::translate("DialogOpenDev", "\346\234\252\346\211\276\345\210\260RTL\350\256\276\345\244\207\357\274\214\350\257\267\346\217\222\345\205\245RTL\350\256\276\345\244\207\345\220\216\345\206\215\345\260\235\350\257\225\345\210\267\346\226\260\346\214\211\351\222\256\357\274\201", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        btnRefresh->setToolTip(QApplication::translate("DialogOpenDev", "\345\210\267\346\226\260\345\217\257\347\224\250\350\256\276\345\244\207", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnRefresh->setText(QApplication::translate("DialogOpenDev", "...", Q_NULLPTR));
        btnOK->setText(QApplication::translate("DialogOpenDev", "\347\241\256\345\256\232", Q_NULLPTR));
        btnCancel->setText(QApplication::translate("DialogOpenDev", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DialogOpenDev: public Ui_DialogOpenDev {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGOPENDEV_H
