#pragma once

#include <QObject>
#include "rtl-sdr.h"
#include <qtimer.h>



class SDRDevice : public QObject
{
	Q_OBJECT

public:
	SDRDevice(QObject *parent);
	~SDRDevice();
public:
	QTimer *timerPlot;

public:
	QString testRTL();
private:
	QQueue<qreal> *m_QueueBuf;
	QQueue<qreal> *m_QueueBufX;
};
