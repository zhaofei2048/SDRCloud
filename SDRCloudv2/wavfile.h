#pragma once
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

#ifndef WAVFILE_H
#define WAVFILE_H
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QFile>
#include <QAudioFormat>

class WavFile : public QFile
{
public:
	WavFile(QObject *parent = 0);

	bool open(const QString &fileName, bool isRead);
	const QAudioFormat &fileFormat() const;
	quint32 dataLength();
	qint64 headerLength() const;
	bool writeWave(const QString &, const QAudioFormat &format, const char *buffer, qint64 len);
	bool readWave(const QString fileName, char *data, qint64 &len, QAudioFormat &format);
	bool readWave(const QString fileName, qreal *data, qint64 &len, QAudioFormat &format);
	bool getWaveDataSize(const QString fileName, quint64 & len, int &sampleSize);
	bool clearAndWriteWave(const QString &name, const QAudioFormat &format, const char *array, qint64 len);

private:
	quint32 readDataHeader();
	bool readAudioFormat();

private:
	QAudioFormat m_fileFormat;
	qint64 m_headerLength;
	quint32 m_dataLenth;    //数据长度
};

#endif // WAVFILE_H

