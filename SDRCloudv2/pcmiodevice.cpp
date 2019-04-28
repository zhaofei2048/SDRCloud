#include "pcmiodevice.h"
#include "constant.h"

PCMIODevice::PCMIODevice()
{
	m_buffer.resize(DEFAULT_MAX_BUFFER_SIZE);
	m_buffer.fill((char)0);
	this->open(QIODevice::ReadOnly);
}

PCMIODevice::~PCMIODevice()
{
	this->close();
}

qint64 PCMIODevice::readData(char * data, qint64 maxlen)
{
	qint64 len = maxlen > m_buffer.count() ? m_buffer.count() : maxlen;

	memcpy(data, m_buffer.constData(), len);

	return len;
}

qint64 PCMIODevice::writeData(const char * data, qint64 len)
{
	m_buffer.replace(0, len, data);
	return len;
}
