#pragma once
#ifndef DIALOGOPENDEV_H
#define DIALOGOPENDEV_H

#pragma execution_character_set("utf-8")
#include <QDialog>
#include "ui_dialogopendev.h"
QT_FORWARD_DECLARE_CLASS(SDRDevice)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)
class DialogOpenDev : public QDialog
{
	Q_OBJECT

public:
	DialogOpenDev(SDRDevice *dongle, QWidget *parent = Q_NULLPTR);
	~DialogOpenDev();
public:
	void setHint(QString hint);
	void addListItem(QListWidgetItem *item);
	void clearAllItem();
	QListWidgetItem *getSelectedItem();
	void setBtnOKState();
private:	// 私有成员函数
	void openRTL();
private slots:
	void itSelSlot();
	void refreshDevList();
private:
	Ui::DialogOpenDev ui;
private:
	SDRDevice *m_dongle;

};

#endif
