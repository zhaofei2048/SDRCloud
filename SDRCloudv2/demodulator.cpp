#include "demodulator.h"
#include "rtldevice2.h"
Demodulator::Demodulator(RtlDevice *dongle, QObject *parent)
	: QObject(parent),
	m_preQdivI(0.0),
	m_signalLen(0),
	m_dongle(dongle)
{
	worker = new DemodWorker();	// 这个不会被重复执行，所以不用判断是否重复实例化
	worker->moveToThread(&m_demodThread);
	connect(this, &Demodulator::m_demodFMSignal, worker, &DemodWorker::doWork);
	m_demodThread.start();
}

Demodulator::~Demodulator()
{
	m_demodThread.wait();
}

void Demodulator::startDemodFM()
{
	emit m_demodFMSignal(this);
}

void Demodulator::getData(QVector<qreal>& data)
{
	// 考虑这里是否需要一定的条件等待，避免重复读取数据
	m_signalLock.lockForRead();
	data.resize(0);
	for (int i = 0; i < m_signalLen; i++)
	{
		data.append(m_signal[i]);
	}
	m_signalLock.unlock();
}
void Demodulator::getDataByChar(char data[], quint32 len)
{
	// 考虑这里是否需要一定的条件等待，避免重复读取数据
	m_signalLock.lockForRead();
	for (int i = 0; i < m_signalLen; i++)
	{
		data[i] = (char)m_signal[i];
	}
	m_signalLock.unlock();
}

void Demodulator::m_demodFM()
{
	qreal QdivIn_1 = 0.0;
	qreal QdivIn = 0.0;
	qreal numZero = 1e-4;
	quint32 j = 0;
	qreal max = 0.1;
	while (true)
	{
		// 在这里放置停止工作结束线程的条件
		if (m_dongle->getState() == RtlDevice::CANCELLING)
		{
			break;
		}
		
		m_dongle->getData(m_unDemodSignal);	//	这里提取硬件里的原始IQ数据，注意这里有可能阻塞
											// m_unDemodSignal中的数在-128-127之间

		// 注:这种解调算法计算量大，精度高

		QdivIn_1 = m_preQdivI;
		j = 0;
		
		m_signalLock.lockForWrite();
		for (int i = 0; i < m_unDemodSignal.count()-1; i = i + 2)
		{
			QdivIn = m_unDemodSignal[i+1] / (m_unDemodSignal[i] + numZero);	// = Q(n) / I(n)
			m_signal[j] = (QdivIn - QdivIn_1) / (1.0 + QdivIn * QdivIn);
			if (m_signal[j] > max)
			{
				max = m_signal[j];	// 这样归一化是粗暴的
			}
			m_signal[j] = 127.0*m_signal[j] / max;	// 缩小100倍
			j++;
			QdivIn_1 = QdivIn;	// Q(n) / I(n) => Q(n-1) / I(n-1)
		}
		m_signalLen = j;
		m_signalLock.unlock();
		// 这里要考虑是否加个条件变量，避免读者重复读取一样的数据

		m_preQdivI = QdivIn_1;
	}
}


void DemodWorker::doWork(Demodulator *demod)
{
	demod->m_demodFM();
}