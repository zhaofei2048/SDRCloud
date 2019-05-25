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

QT_FORWARD_DECLARE_CLASS(WaveRecorder);

class PCMIODevice : public QIODevice
{
	Q_OBJECT

public:
	
	PCMIODevice(WaveRecorder *recorder);
	~PCMIODevice();
	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len); //它是个纯虚函数， 不得不实现
	void stopUpdate();
	void setRecordState(bool on);
private:
	/*QByteArray m_buffer;*/
	char m_buffer[DEFAULT_MAX_AUDIO_BUFFER_SIZE];
	qint64 curRead;	// 在buffer中当前应读写的位置
	qint64 curWrite;
	quint64 validDataLength;
	WaveRecorder *m_recorder;
	bool isRecordOn;

	// 不必要的锁
	QMutex mutex;
	QWaitCondition notForRead;
	QWaitCondition notForWrite;
	bool isForWrite;
};

#endif // !PCMIODEVICE_H