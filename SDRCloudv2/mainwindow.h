#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")


#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QQueue>
#include <stdio.h>
#include <stdlib.h>
#include <QThread>
#include <QReadWriteLock>
#include <QBuffer>
#include <QAudioOutput>


QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
class QRadioButton;
class QDoubleSpinBox;
class QSlider;
class QCheckBox;
class QComboBox;
QT_END_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QCustomPlot)
//QT_FORWARD_DECLARE_CLASS(SDRDevice)
QT_FORWARD_DECLARE_CLASS(RtlDevice)
QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QToolBox)
QT_FORWARD_DECLARE_CLASS(Demodulator)
QT_FORWARD_DECLARE_CLASS(QAudioOutput)
QT_FORWARD_DECLARE_CLASS(PCMIODevice)
class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	static const QString demodMethodNames[8];
public:
	MainWindow(FILE *logFile, QWidget *parent = Q_NULLPTR);
	~MainWindow();
	friend class UpdateFigureBufWorker;
	friend class UpdateAudioBufWorker;

private:	// 私有成员函数
	void iniUI();
	void iniLeftToolBoxUI();
	void iniStatusBar();
	void connectSignalSlot();
	void console(const QString &info);
	void displayDefaultConfig();	// 将默认参数显示到界面组件上  
	void initFigure();	// 此函数在程序声明周期类，只应该执行1次
	void initAudioPlayer();
	void stopReceiveData();
	void startReceiveData();
	void startAudioPlay();
	void stopAudioPlay();
	
private:	// 这些函数不要直接调用，请调用其接口函数
	void updateFigureBuf();	// 此函数会阻塞
	void updateAudioBuf();	// 此函数会阻塞不应该直接调用
private slots:	//私有槽函数
	void openRTL(bool b);
	void closeRTL(bool b);
	void updatePsdWave();	// 更新功率谱图
	void showMenuRTL();
	void btnStartPlaySlot();	// 开始播放按钮槽函数
	void handleStateChanged(QAudio::State newState);
signals:
	void updateFigureBufSignal(MainWindow *w);
	void updateAudioBufSignal(MainWindow *w);
private:	// 界面组件成员变量
	Ui::MainWindowClass ui;
	// 需要一直保存的界面组件
	QWidget *div;	// 中心容器
	QDockWidget *dockConsole;	// 荣纳控制台的浮动容器
	QDockWidget *dockWaterfall;	// 容纳瀑布图的浮动容器
	QDockWidget *dockLeft;	// 左侧浮动容器
	QTextEdit *txtConsole;	// 输出控件
	QCustomPlot *figure;
	QToolBox *leftToolBox;
	QPushButton *btnStartPlay;
	QRadioButton *rbtnDemodMode[8];
	QDoubleSpinBox *sboxTunerFreq;	// 设置tuner的频率
	QSlider *slideRFGain;
	QSlider *slideIFGain;
	QCheckBox *chkboxOffsetTuning;
	QCheckBox *chkboxTunerAGC;	// 这个控制的本质上是tuner的增益到底是手工控制还是自动控制(manual or automatic)
	QCheckBox *chkboxRtlAGC;
	QComboBox *comboxSampleRate;
	QComboBox *comboxSampleMode;
	QLabel *labDeviceName;
	

private:	// 功能性成员变量
	//SDRDevice *m_dongle;	// SDR硬件即软件狗
	//QQueue<qreal> *m_QueueBufX;	// 数据缓冲队列
	//QQueue<qreal> *m_QueueBufY;
	//QVector<qreal> *m_bufX;	// 数据缓冲队列（vector形式），暂时设立，为了图形的显示
	//QVector<qreal> *m_bufY;
	QTimer *updateFigureTimer;
	RtlDevice *m_dongle;	// RTL硬件对象
	Demodulator *m_demod;	// 解调器对象
	FILE *m_logFile;
	QQueue<qreal> m_figureBufX;
	QQueue<qreal> m_figureBufY;
	QVector<qreal> m_figureX;
	QVector<qreal> m_figureY;
	QReadWriteLock m_figLock;
	quint32 m_figureBufLen;
	QThread updateFigureBufThread;	// 更新figure的buffer的线程
	UpdateFigureBufWorker *updateFigureBufWorker;
	UpdateAudioBufWorker *updateAudioBufWorker;
	QThread updateAudioBufThread;	// 更新audio的buffer的线程
	// 声音播放
	QAudioOutput *m_audio;
	/*QBuffer m_audioBuffer;*/
	PCMIODevice *m_pcmIODevice;
};



class UpdateFigureBufWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(MainWindow *w);
signals:
	void done();
};


class UpdateAudioBufWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(MainWindow *w);
signals:
	void done();
};
#endif // MAINWINDOW_H