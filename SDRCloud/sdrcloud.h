#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_sdrcloud.h"
#include <qcustomplot.h>
#include "sdrdevice.h"
#include <QPlainTextEdit>
#include <QToolBox>
#include "audioiodevice.h"
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <qDebug>
#include <QDockWidget>
QT_BEGIN_NAMESPACE
class QAudioInput;
QT_END_NAMESPACE
class SDRCloud : public QMainWindow
{
	Q_OBJECT

public:
	SDRCloud(QWidget *parent = Q_NULLPTR);
	~SDRCloud();

private:
	Ui::SDRCloudClass ui;
	// 通过代码摆放的工作区组件
	QCustomPlot *ploter;
	QPlainTextEdit *txtEditConsole;
	QToolBox *leftToolBox;


private:
	quint16 m_buflen;
	QVector<qreal> *m_vecSample;
	QVector<qreal> *m_sampleX;
	QQueue<qreal> *m_QueueBuf;
	QQueue<qreal> *m_QueueBufX;
	AudioIODevice *m_audioIODevice;
	QAudioInput *m_audioInput;
	
public:
	SDRDevice *rtl;
	QTimer *myTimer;

public:
	void iniUI();
	void printToConsole(const QString &s);
	void test();

	void updateWave(const QVector<qreal> data, quint16 len);
public slots:
	void updateWaveSlot();
};
