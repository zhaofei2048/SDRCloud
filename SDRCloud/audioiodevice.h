#pragma once

#include <QObject>
#include <QIODevice>
#include <QVector>
#include <QQueue>
#include <qDebug>


class AudioIODevice : public QIODevice
{
	Q_OBJECT

public:
	explicit AudioIODevice(QObject *parent, QVector<qreal> *vecSample, QVector<qreal> *sampleX, QQueue<qreal> *queueBuf, QQueue<qreal> *queueBufX);
	~AudioIODevice();
protected:
	qint64 readData(char * data, qint64 maxSize);
	qint64 writeData(const char * data, qint64 maxSize);
private:
	QVector<qreal> *m_vecSample;
	QVector<qreal> *m_sampleX;
	QQueue<qreal> *m_QueueBuf;
	QQueue<qreal> *m_QueueBufX;
};
