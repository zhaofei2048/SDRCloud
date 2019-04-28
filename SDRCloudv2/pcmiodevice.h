#pragma once
#ifndef PCMIODEVICE_H
#define PCMIODEVICE_H


#include <QObject>
#include <QIODevice>
#include <QByteArray>

class PCMIODevice : public QIODevice
{
	Q_OBJECT

public:
	PCMIODevice();
	~PCMIODevice();
	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len); //它是个纯虚函数， 不得不实现
private:
	QByteArray m_buffer;


};

#endif // !PCMIODEVICE_H