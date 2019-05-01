#pragma once

#include <QObject>
#include <QQueue>
#include <QThread>
#define QCUSTOMPLOT_USE_LIBRARY
#include "qcustomplot.h"

QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(Demodulator)
class Figure : public QCustomPlot
{
	Q_OBJECT

public:
	Figure(Demodulator *demod, QWidget *parent=nullptr);
	~Figure();
	friend class UpdateFigureBufWorker;

	void startRedrawing();
	void stopRedrawing();
	

private:
	void initFigure();	// 此函数在程序声明周期类，只应该执行1次
	void m_updateFigureBuf();	// 此函数会阻塞
	void startUpdateFigureBuf();	///< 接收数据到达信号对figure的buf进行更新,，没有数据时会自己停下
private slots:
	void updatePsdWave();	// 更新功率谱图
signals:
	void updateFigureBufSignal(Figure *fig);
private:
	Demodulator *m_demod;
	QTimer *redrawingFigureTimer;
	QQueue<qreal> m_figureBufX;
	QQueue<qreal> m_figureBufY;
	QVector<qreal> m_figureX;
	QVector<qreal> m_figureY;
	QReadWriteLock m_figLock;
	quint32 m_figureBufLen;
	QThread updateFigureBufThread;	// 更新figure的buffer的线程
	UpdateFigureBufWorker *updateFigureBufWorker;
};


class UpdateFigureBufWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(Figure *fig);
signals:
	void done();
};