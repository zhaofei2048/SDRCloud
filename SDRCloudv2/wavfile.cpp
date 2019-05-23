/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qendian.h>
#include <QVector>
#include <QDebug>
#include "wavfile.h"

struct chunk
{
	char        id[4];
	quint32     size;
};

struct RIFFHeader
{
	chunk       descriptor;     // "RIFF"
	char        type[4];        // "WAVE"
};

struct WAVEHeader
{
	chunk       descriptor;
	quint16     audioFormat;
	quint16     numChannels;
	quint32     sampleRate;
	quint32     byteRate;
	quint16     blockAlign;
	quint16     bitsPerSample;
};

struct DATAHeader
{
	chunk       descriptor;
};

struct CombinedHeader
{
	RIFFHeader  riff;
	WAVEHeader  wave;
};

WavFile::WavFile(QObject *parent)
	: QFile(parent)
	, m_headerLength(0)
{

}
//isRead == true为读方式， ==false 为写方式

bool WavFile::open(const QString &fileName, bool isRead)
{
	bool result;
	if (this->isOpen())
		QFile::close();
	setFileName(fileName);
	if (isRead)
		result = QFile::open(QIODevice::ReadOnly) && readAudioFormat();
	else
		result = QFile::open(QIODevice::ReadWrite);

	return result;
}

const QAudioFormat &WavFile::fileFormat() const
{
	return m_fileFormat;
}

qint64 WavFile::headerLength() const
{
	return m_headerLength;
}

//读取数据头，返回数据大小
quint32 WavFile::readDataHeader()
{
	quint32 datasize = 0;
	seek(0);
	CombinedHeader header;
	DATAHeader dataHeader;
	bool result = read(reinterpret_cast<char *>(&header), sizeof(CombinedHeader)) == sizeof(CombinedHeader);
	if (result) {
		if ((memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0
			|| memcmp(&header.riff.descriptor.id, "RIFX", 4) == 0)
			&& memcmp(&header.riff.type, "WAVE", 4) == 0
			&& memcmp(&header.wave.descriptor.id, "fmt ", 4) == 0
			&& (header.wave.audioFormat == 1 || header.wave.audioFormat == 0)) {

			if (read((char*)&dataHeader, sizeof(DATAHeader)) == sizeof(DATAHeader))
				datasize = dataHeader.descriptor.size;
		}
	}
	return datasize;
}

//读header，读取audio format
bool WavFile::readAudioFormat()
{
	seek(0);
	CombinedHeader header;
	bool result = read(reinterpret_cast<char *>(&header), sizeof(CombinedHeader)) == sizeof(CombinedHeader);
	if (result) {
		if ((memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0
			|| memcmp(&header.riff.descriptor.id, "RIFX", 4) == 0)
			&& memcmp(&header.riff.type, "WAVE", 4) == 0
			&& memcmp(&header.wave.descriptor.id, "fmt ", 4) == 0
			&& (header.wave.audioFormat == 1 || header.wave.audioFormat == 0)) {

			if (qFromLittleEndian<quint32>(header.wave.descriptor.size) > sizeof(WAVEHeader)) {
				// Extended data available
				quint16 extraFormatBytes;
				if (peek((char*)&extraFormatBytes, sizeof(quint16)) != sizeof(quint16))
					return false;
				const qint64 throwAwayBytes = sizeof(quint16) + qFromLittleEndian<quint16>(extraFormatBytes);
				if (read(throwAwayBytes).size() != throwAwayBytes)
					return false;
			}
			DATAHeader dataHeader;
			if (read((char*)&dataHeader, sizeof(DATAHeader)) == sizeof(DATAHeader))
				m_dataLenth = dataHeader.descriptor.size;
			// Establish format
			if (memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0)
				m_fileFormat.setByteOrder(QAudioFormat::LittleEndian);
			else
				m_fileFormat.setByteOrder(QAudioFormat::BigEndian);

			int bps = qFromLittleEndian<quint16>(header.wave.bitsPerSample);
			m_fileFormat.setChannelCount(qFromLittleEndian<quint16>(header.wave.numChannels));
			m_fileFormat.setCodec("audio/pcm");
			m_fileFormat.setSampleRate(qFromLittleEndian<quint32>(header.wave.sampleRate));
			m_fileFormat.setSampleSize(qFromLittleEndian<quint16>(header.wave.bitsPerSample));
			m_fileFormat.setSampleType(bps == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);
		}
		else {
			result = false;
		}
	}
	m_headerLength = pos();
	return result;
}

bool WavFile::writeWave(const QString &name, const QAudioFormat &format, const char *array, qint64 len)
{
	bool result = this->open(name, false);

	if (result == false) {
		qDebug() << "file not open";
	}
	//quint32 datasize = readDataHeader();	// 开启条语句代替下面语句用于改为wav文件的【追加模式】，即多次向同一个文件追加音频内容
	quint32 datasize = 0;	// 这里总是让其【覆盖原来】存在的wav文件
	if (datasize == 0)
	{
		CombinedHeader header;
		memcpy(&header.riff.descriptor.id, "RIFF", 4);
		header.riff.descriptor.size = qint32(len) + 36;	// header.riff.descriptor.size = qint32(len) + 32
		memcpy(&header.riff.type, "WAVE", 4);
		memcpy(&header.wave.descriptor.id, "fmt ", 4);
		header.wave.descriptor.size = 16;
		header.wave.audioFormat = 1;//"PCM"
		header.wave.numChannels = format.channelCount();

		header.wave.sampleRate = format.sampleRate();
		header.wave.bitsPerSample = format.sampleSize();	// header.wave.bitsPerSample = 16
		header.wave.blockAlign = header.wave.numChannels * header.wave.bitsPerSample / 8;
		header.wave.byteRate = header.wave.blockAlign * header.wave.sampleRate;

		seek(0);
		result &= (write(reinterpret_cast<char *>(&header), sizeof(CombinedHeader)) == sizeof(CombinedHeader));
	}
	// Read off remaining header information
	DATAHeader dataHeader;
	memcpy(&dataHeader.descriptor.id, "data", 4);
	dataHeader.descriptor.size = qint32(len + datasize);


	seek(36);
	result &= (write(reinterpret_cast<char *>(&dataHeader), sizeof(DATAHeader)) == sizeof(DATAHeader));
	seek(44 + datasize);

	result &= (write(array, len) == len);

	close();
	return result;
}
quint32 WavFile::dataLength()
{
	return m_dataLenth;
}
