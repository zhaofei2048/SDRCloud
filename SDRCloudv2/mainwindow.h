#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")


#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QQueue>


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
QT_END_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QCustomPlot)
QT_FORWARD_DECLARE_CLASS(SDRDevice)
QT_FORWARD_DECLARE_CLASS(QTimer)
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
private:	// 私有成员函数
	void iniUI();
	void connectSignalSlot();
	void console(const QString &info);
private slots:	//私有槽函数
	void openRTL(bool b);
	void closeRTL(bool b);
	void updateWave();
	void showMenuRTL();

private:	// 界面组件成员变量
	Ui::MainWindowClass ui;
	// 需要一直保存的界面组件
	QWidget *div;	// 中心容器
	QDockWidget *dockConsole;	// 荣纳控制台的浮动容器
	QDockWidget *dockWaterfall;	// 容纳瀑布图的浮动容器
	QDockWidget *dockLeft;	// 左侧浮动容器
	QTextEdit *txtConsole;	// 输出控件
	QCustomPlot *figure;

private:	// 功能性成员变量
	SDRDevice *m_dongle;	// SDR硬件即软件狗
	QQueue<qreal> *m_QueueBufX;	// 数据缓冲队列
	QQueue<qreal> *m_QueueBufY;
	QVector<qreal> *m_bufX;	// 数据缓冲队列（vector形式），暂时设立，为了图形的显示
	QVector<qreal> *m_bufY;
	QTimer *updWaveTimer;
};
#endif // MAINWINDOW_H