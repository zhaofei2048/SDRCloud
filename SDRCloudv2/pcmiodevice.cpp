#include "pcmiodevice.h"
#include "constant.h"
#include <QDebug>


PCMIODevice::PCMIODevice():
	curRead(0),
	curWrite(0),
	isForWrite(true)
{
	for (int i = 0; i < DEFAULT_MAX_AUDIO_BUFFER_SIZE; i++)
	{
		m_buffer[i] = 2;
	}
	this->open(QIODevice::ReadOnly);
}

PCMIODevice::~PCMIODevice()
{
	this->close();
}

qint64 PCMIODevice::readData(char * data, qint64 maxlen)
{
	qint64 chunk = 0;
	qint64 pos = 0;	// data的当前位置指针
	/*qDebug() << "audio read data" + QString::number(maxlen);*/
	
	mutex.lock();
	if (isForWrite == true)
	{
		notForWrite.wait(&mutex);	// 这个轮流锁主要是为了限制执行得太快的writeData操作
									// 同时这里是有可能出现读者（写者）离去，而另一个被阻塞的。
	}
	mutex.unlock();

	for (qint64 len = maxlen; len > 0; pos += chunk, len -= chunk)
	{
		chunk = len > (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curRead) ? (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curRead) : len;
		memcpy(data+pos, m_buffer+curRead, chunk);
		curRead = (curRead + chunk) % DEFAULT_MAX_AUDIO_BUFFER_SIZE;	
	}

	mutex.lock();
	isForWrite = true;
	notForRead.wakeAll();
	mutex.unlock();

	return pos;
}

qint64 PCMIODevice::writeData(const char * data, qint64 len)
{
	qint64 chunk = 0;
	qint64 pos = 0;	// data的当前位置指针
	/*qDebug() << "write audio data:";*/
	mutex.lock();
	if (isForWrite == false)
	{
		notForRead.wait(&mutex);
	}
	mutex.unlock();
	/*qDebug() << data[2];*/
	for (qint64 ilen = len; ilen > 0; pos += chunk, ilen -= chunk)
	{
		chunk = ilen > (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curWrite) ? (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curWrite) : ilen;
		memcpy(m_buffer + curWrite, data + pos, chunk);
		curWrite = (curWrite + chunk) % DEFAULT_MAX_AUDIO_BUFFER_SIZE;
	}

	mutex.lock();
	isForWrite = false;
	notForWrite.wakeAll();
	mutex.unlock();

	return pos;
}

//qint64 PCMIODevice::readData(char * data, qint64 maxlen)
//{
//	qint64 len = maxlen > DEFAULT_MAX_AUDIO_BUFFER_SIZE ? DEFAULT_MAX_AUDIO_BUFFER_SIZE : maxlen;
//	qDebug() << "maxlen=" + QString::number(maxlen);
//	qDebug() << QString::asprintf("\nm_buffer:%d,%d,%d,%d", data[2], data[10], data[50], data[1000]);
//	memcpy(data, m_buffer, len);
//	qDebug() << QString::asprintf("\n%d,%d,%d,%d", data[2], data[10], data[50], data[1000]);
//	return len;
//}
//
//qint64 PCMIODevice::writeData(const char * data, qint64 len)
//{
//	/*m_buffer.replace(0, len, data);*/
//	/*qDebug() << "write audio data";*/
//	memcpy(m_buffer, data, len);
//	qDebug() << QString::number(len);
//	//qDebug() << QString::asprintf("\ndataIn:%d,%d,%d,%d", data[2], data[10], data[50], data[1000]);
//	qDebug() << QString::asprintf("\nm_bufferIn:%d,%d,%d,%d", data[2], data[10], data[50], data[1000]);
//	return len;
//}