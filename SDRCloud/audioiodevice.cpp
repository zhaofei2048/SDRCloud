#include "audioiodevice.h"

AudioIODevice::AudioIODevice(QObject *parent, QVector<qreal> *vecSample, QVector<qreal> *sampleX, QQueue<qreal> *queueBuf, QQueue<qreal> *queueBufX)
	: QIODevice(parent),
	m_vecSample(vecSample),
	m_sampleX(sampleX),
	m_QueueBuf(queueBuf),
	m_QueueBufX(queueBufX)
{
}

AudioIODevice::~AudioIODevice()
{
}

qint64 AudioIODevice::readData(char * data, qint64 maxSize)
{
	Q_UNUSED(data)
	Q_UNUSED(maxSize)
	return -1;
}

qint64 AudioIODevice::writeData(const char * data, qint64 maxSize)
{
	qint64 range = 2000;
	QVector<qreal> oldSample;
	QVector<qreal> sample;
	int resolution = 4;


	//int count = m_QueueBuf->count();


	////=================2
	// 下面有一个问题：写成for (int i = m_QueueBuf->count(); i > range; i--)就会运行时说访问冲突
	// 发现了，是因为没有初始化变量
	for (int i = m_QueueBuf->count(); i > range; i--)
	{
		if (!m_QueueBuf->empty())
		m_QueueBuf->dequeue();
	}
	for (int i = 0; i < maxSize / resolution; i++)	// (qreal)((quint8)(data[resolution * i] - 128) / 128.0)
	{
		m_QueueBuf->enqueue((qreal)((quint8)data[resolution * i] - 128) / 128.0);
	}

	int count2 = m_QueueBuf->count();

	while (!m_QueueBufX->empty())m_QueueBufX->dequeue();
	for (int i = 0; i < count2; i++)
	{
		m_QueueBufX->enqueue(i);
	}
	

	//================3
	//oldSample = QVector<qreal>::QVector(*m_vecSample);

	//if (oldSample.count() < range)
	//{	// 说明缓冲区有空可以插入
	//	sample = QVector<qreal>::QVector(*m_vecSample);	// 其实不用交换，以后改成仅拷贝赋值
	//}
	//else
	//{	// 说明缓冲区不够大，得清掉一些
	//	for (int i = maxSize / resolution; i < oldSample.count(); i++)
	//	{
	//		sample.append(oldSample[i]);
	//	}
	//}
	//qint64 size = sample.count();
	//for (int k = 0; k < maxSize / resolution; k++)
	//{
	//	sample.append((qreal)((quint8)(data[resolution * k] - 128) / 128.0));
	//}
	//for (int i = 0; i < range; i++)
	//{
	//	m_sampleX->append(i);
	//}
	//*m_vecSample = QVector<qreal>::QVector(sample);
	return maxSize;
}
