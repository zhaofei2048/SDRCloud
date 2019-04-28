#pragma once
#ifndef DEMODULATOR_H
#define DEMODULATOR_H

#include <QObject>
#include "constant.h"
#include <QVector>
#include <QReadWriteLock>
#include <QThread>

QT_FORWARD_DECLARE_CLASS(RtlDevice)

// 解调器类，负责对信号的数字解调
class Demodulator : public QObject
{
	Q_OBJECT

public:
	Demodulator(RtlDevice *dongle, QObject *parent = nullptr);
	~Demodulator();
	friend class DemodWorker;
public:	// 暴露出去的接口
	void startDemodFM();	// 请确保硬件正常工作时再调用我进行解调，否则可能出现线程一直阻塞下去的错误
	void getData(QVector<qreal> &data);	// 获取解调后的数据
	void getDataByChar(char data[], quint32 len);

private:
	void m_demodFM();
signals:
	void m_demodFMSignal(Demodulator *demod);

private:
	QVector<qreal> m_unDemodSignal;
	qreal m_signal[DEFAULT_MAX_BUFFER_SIZE];	// 存放解调后的结果
	quint32 m_signalLen;	// 当前信号长度
	QReadWriteLock m_signalLock;	// 读写锁
	qreal m_preQdivI;	// Q(n-1)/I(n-1)
	RtlDevice *m_dongle;
	DemodWorker *worker;
	QThread m_demodThread;
};


class DemodWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(Demodulator *demod);
signals:
	void done();
};

#endif // !DEMODULATOR_H