#include "pcmiodevice.h"
#include "constant.h"
#include <QDebug>
#include "waverecorder.h"


PCMIODevice::PCMIODevice(WaveRecorder *recorder) :
	curRead(0),
	curWrite(0),
	isForWrite(true),
	validDataLength(0),
	m_recorder(recorder),
	isRecordOn(false)
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

	qint64 pre_curRead = curRead;
	//qDebug() << "read max len = " << maxlen;
	
	mutex.lock();
	if (isForWrite == true)
	{
		notForWrite.wait(&mutex);	// 这个轮流锁主要是为了限制执行得太快的writeData操作
									// 同时这里是有可能出现读者（写者）离去，而另一个被阻塞的。
	}
	mutex.unlock();

	if (maxlen > validDataLength)
	{
		maxlen = validDataLength;	// 如果剩余有效数据不够
	}
	for (qint64 len = maxlen; len > 0; pos += chunk, len -= chunk)
	{
		chunk = len > (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curRead) ? (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curRead) : len;
		memcpy(data+pos, m_buffer+curRead, chunk);
		curRead = (curRead + chunk) % DEFAULT_MAX_AUDIO_BUFFER_SIZE;	
	}

	validDataLength -= pos;
	// 读取到的音频数据都在data里，且其长度为pos
	//for (int i = 0; i < pos; i++) {
	//	data[i] = char(100 * cos(2 * 3.14 * 1200 * i / 48000.0));
	//	//data[i] = char(-30);
	//}

	for (quint32 i = 0; i < pos; i++) {	// 这里主要是为了将有符号数转为无符号数。
		if (data[i] >= 0) {
			data[i] = data[i] - 128;
		}
		else {
			data[i] = data[i] + 128;
		}
	}
	if (isRecordOn) {
		m_recorder->cacheWave(data, pos);	// 对音 频数据进行缓存
	}
	Q_ASSERT((pre_curRead + pos) % DEFAULT_MAX_AUDIO_BUFFER_SIZE == curRead);
	mutex.lock();
	isForWrite = true;
	notForRead.wakeAll();
	mutex.unlock();

	if (pos == 0)
	{
		data[0] = (char)-128;
		pos = 1;	// 不能让扬声器进入休眠状态，不然写者会被孤独的锁住
		qDebug() << "player is free";
	}
	//qDebug() << QString::number(pos) + data[0];
	//qDebug() << "really read len = " << pos;
	return pos;
}
//char test_data[16384];
qint64 PCMIODevice::writeData(const char * data, qint64 len)
{
	qint64 chunk = 0;
	qint64 pos = 0;	// data的当前位置指针


	qint64 pre_curWrite = curWrite;
	//qDebug() << "write len = " << len;
	mutex.lock();
	if (isForWrite == false)
	{
		notForRead.wait(&mutex);
	}
	mutex.unlock();

	for (qint64 ilen = len; ilen > 0; pos += chunk, ilen -= chunk)
	{
		chunk = ilen > (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curWrite) ? (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curWrite) : ilen;
		memcpy(m_buffer + curWrite, data + pos, chunk);
		curWrite = (curWrite + chunk) % DEFAULT_MAX_AUDIO_BUFFER_SIZE;
	}
	validDataLength += pos;

	Q_ASSERT(len == pos);
	Q_ASSERT((pre_curWrite + pos) % DEFAULT_MAX_AUDIO_BUFFER_SIZE == curWrite);
	mutex.lock();
	isForWrite = false;
	notForWrite.wakeAll();
	mutex.unlock();

	return pos;
}

void PCMIODevice::stopUpdate()
{
	notForRead.wakeAll();
	notForWrite.wakeAll();
}

void PCMIODevice::setRecordState(bool on)
{
	isRecordOn = on;
}
