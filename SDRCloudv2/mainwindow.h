#pragma once
/*
	程序主窗口类
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")


#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
//#include <QQueue>
#include <stdio.h>
#include <stdlib.h>
//#include <QThread>
//#include <QReadWriteLock>
//#include <QBuffer>
//#include <QAudioOutput>


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
class QProgressBar;
QT_END_NAMESPACE
//QT_FORWARD_DECLARE_CLASS(QCustomPlot)
//QT_FORWARD_DECLARE_CLASS(SDRDevice)
QT_FORWARD_DECLARE_CLASS(Figure)
QT_FORWARD_DECLARE_CLASS(RtlDevice)
//QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QToolBox)
QT_FORWARD_DECLARE_CLASS(Demodulator)
//QT_FORWARD_DECLARE_CLASS(QAudioOutput)
//QT_FORWARD_DECLARE_CLASS(PCMIODevice)
QT_FORWARD_DECLARE_CLASS(SoundPlayer)
QT_FORWARD_DECLARE_CLASS(ImageViewer)
class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	static const QString demodMethodNames[8];
public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
	friend class UpdateFigureBufWorker;
	
private:	// 私有成员函数
	void iniUI();
	void iniLeftToolBoxUI();
	void iniStatusBar();
	void connectSignalSlot();
	void console(const QString &info);
	void displayDefaultConfig();	// 将默认参数显示到界面组件上  
	void stopRtl();
	void startRtl();

private slots:	//私有槽函数
	void openRTL(bool b);
	void closeRTL(bool b);
	//void updatePsdWave();	// 更新功率谱图
	void showMenuRTL();
	void btnStartPlaySlot();	// 开始播放按钮槽函数
	void lostDevice(QString info);
	void openImgWindow(bool b);

signals:
	/*to do*/

private:	// 界面组件成员变量
	Ui::MainWindowClass ui;
	// 需要一直保存的界面组件
	QWidget *div;	// 中心容器
	QDockWidget *dockConsole;	// 荣纳控制台的浮动容器
	QDockWidget *dockWaterfall;	// 容纳瀑布图的浮动容器
	QDockWidget *dockLeft;	// 左侧浮动容器
	QTextEdit *txtConsole;	// 输出控件
	Figure *figure;	/*QCustomPlot *figure;*/
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
	QComboBox *comboxRecord;
	QProgressBar *pgbRecord;	// 显示录制时长的进度条
	QPushButton *btnClearRecord; // 清空已录制内容
	QLabel *labDeviceName;
	ImageViewer *imgViewer;

	

private:	// 功能性成员变量
	RtlDevice *m_dongle;	// RTL硬件对象
	Demodulator *m_demod;	// 解调器对象
	SoundPlayer *m_player;	// 播放器
	//FILE *m_logFile;	// Debug日志文件
	quint32 m_recordTime;	// 音频录制时间
	quint32 m_currentRecordTime;	// 已录制音频时间
};







#endif // MAINWINDOW_H