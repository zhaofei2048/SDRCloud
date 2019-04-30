#pragma once
/*
	音频流输入输出类
*/
#ifndef PCMIODEVICE_H
#define PCMIODEVICE_H
#include "constant.h"

#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QMutex>
#include <QWaitCondition>

class PCMIODevice : public QIODevice
{
	Q_OBJECT

public:
	
	PCMIODevice();
	~PCMIODevice();
	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len); //它是个纯虚函数， 不得不实现
private:
	/*QByteArray m_buffer;*/
	char m_buffer[DEFAULT_MAX_AUDIO_BUFFER_SIZE];
	qint64 curRead;	// 在buffer中当前应读写的位置
	qint64 curWrite;

	// 不必要的锁

	QMutex mutex;
	QWaitCondition notForRead;
	QWaitCondition notForWrite;
	bool isForWrite;
};

#endif // !PCMIODEVICE_H