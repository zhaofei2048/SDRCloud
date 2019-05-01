#include "demodulator.h"
#include "rtldevice.h"
#include "qmath.h"
#include "demod_core.h"

Demodulator::Demodulator(RtlDevice *dongle, QObject *parent)
	: QObject(parent),
	m_preQdivI(0.0),
	m_signalLen(0),
	m_dongle(dongle)
{
	demodWorker = new DemodWorker();	// 这个不会被重复执行，所以不用判断是否重复实例化
	demodWorker->moveToThread(&m_demodThread);
	connect(this, &Demodulator::m_demodFMSignal, demodWorker, &DemodWorker::doWork);
	m_demodThread.start();
}

Demodulator::~Demodulator()
{
	m_demodThread.quit();
	m_demodThread.wait();
}

void Demodulator::startDemodFM()
{
	m_isRunning = true;
	emit m_demodFMSignal(this);
}

void Demodulator::stopDemodFM()
{
	m_isRunning = false;
}

bool Demodulator::isRunning()
{
	return m_isRunning;
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

void Demodulator::getDataByChar(char data[], quint32 &len)
{
	// 考虑这里是否需要一定的条件等待，避免重复读取数据
	m_signalLock.lockForRead();
	int i = 0;
	for (i = 0; i < m_signalLen; i++)
	{
		data[i] = char(m_signal[i]);
	}
	len = i;
	m_signalLock.unlock();
}

void Demodulator::m_demodFM()
{
	qreal QdivIn_1 = 0.0;
	qreal QdivIn = 0.0;
	const qreal numZero = 1e-4;
	static double data_in[DEFAULT_MAX_BUFFER_SIZE];
	static double data_out[DEFAULT_MAX_BUFFER_SIZE];
	unsigned int len_in = 0;
	unsigned int len_out = 0;

	qreal max = 0.1;

	while (true)
	{
		//qDebug() << "is demoding";
		// 在这里放置停止工作结束线程的条件
		if (m_dongle->getState() == RtlDevice::CANCELING)
		{
			m_isRunning = false;
			qDebug() << "jump out demoding";
			break;
		}

		m_dongle->getData(m_unDemodSignal);	//	这里提取硬件里的原始IQ数据，注意这里有可能阻塞
											// m_unDemodSignal中的数在-128-127之间
		// 注:这种解调算法计算量大，精度高

		for (int i = 0; i < m_unDemodSignal.count(); i++)
		{
			data_in[i] = m_unDemodSignal[i];
		}
		len_in = m_unDemodSignal.count();
		demod_core(data_in, len_in, data_out, &len_out);

		for (int i = 0; i < m_unDemodSignal.count(); i++)
		{
			data_in[i] = m_unDemodSignal[i];
		}

		m_signalLock.lockForWrite();
		for (int i = 0; i < len_out; i++)
		{
			m_signal[i] = data_out[i];
		}
		m_signalLen = len_out;
		m_signalLock.unlock();
	}
}

//void Demodulator::m_demodFM()
//{
//	qreal QdivIn_1 = 0.0;
//	qreal QdivIn = 0.0;
//	const qreal numZero = 1e-4;
//	
//	qreal max = 0.1;
//
//	while (true)
//	{
//		//qDebug() << "is demoding";
//		// 在这里放置停止工作结束线程的条件
//		if (m_dongle->getState() == RtlDevice::CANCELING)
//		{
//			m_isRunning = false;
//			qDebug() << "jump out demoding";
//			break;
//		}
//
//		m_dongle->getData(m_unDemodSignal);	//	这里提取硬件里的原始IQ数据，注意这里有可能阻塞
//											// m_unDemodSignal中的数在-128-127之间
//		// 注:这种解调算法计算量大，精度高
//
//		QdivIn_1 = m_preQdivI;
//		quint32 j = 0;
//		m_signalLock.lockForWrite();
//		for (int i = 0; i < m_unDemodSignal.count() - 1; i = i + 2)
//		{
//			QdivIn = m_unDemodSignal[i + 1] / (m_unDemodSignal[i] + numZero);	// = Q(n) / I(n)
//			m_signal[j] = (QdivIn - QdivIn_1) / (1.0 + QdivIn * QdivIn);
//			if (m_signal[j] > max)
//			{
//				max = m_signal[j];	// 这样归一化是粗暴的
//			}
//			m_signal[j] = 127.0*m_signal[j] / max;	// 缩小100倍
//			j++;
//			QdivIn_1 = QdivIn;	// Q(n) / I(n) => Q(n-1) / I(n-1)
//		}
//
//		m_signalLen = j;
//		m_signalLock.unlock();
//		m_preQdivI = QdivIn_1;
//	}
//}


	//for (int i = 0; i < m_unDemodSignal.count() - 1; i = i + 2)
	//{
	//	//QdivIn = m_unDemodSignal[i + 1] / (m_unDemodSignal[i] + numZero);	// = Q(n) / I(n)
	//	//m_signal[j] = (QdivIn - QdivIn_1) / (1.0 + QdivIn * QdivIn);
	//	//if (m_signal[j] > max)
	//	//{
	//	//	max = m_signal[j];	// 这样归一化是粗暴的
	//	//}
	//	//m_signal[j] = 127.0*m_signal[j] / max;	// 缩小100倍
	//	m_signal[j] = 50.0*qCos(2.0*3.14*0.003*j)+ m_unDemodSignal[1];
	//	j++;
	//	//QdivIn_1 = QdivIn;	// Q(n) / I(n) => Q(n-1) / I(n-1)
	//}

	// 这里要考虑是否加个条件变量，避免读者重复读取一样的数据



void DemodWorker::doWork(Demodulator *demod)
{
	demod->m_demodFM();
}