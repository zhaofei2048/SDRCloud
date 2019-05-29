#pragma once
#ifndef WAVEFILE_H
#define WAVEFILE_H


#include <QObject>
#include <QAudioFormat>
#include "wavfile.h"
//#define WAVE_FORMAT_PCM 1
#define WAVE_FILE_BUFFER_SIZE 	10*16777216 // 40MB
//typedef quint32 DWORD;
//typedef quint16 WORD;
//
//typedef struct {
//	char riffID[4];	//	"RIFF"				4B
//	DWORD riffSize;	//						4B
//	char riffType[4];	//	"WAVE"			4B
//
//	char fmtID[4];	//	"fmt"				4B
//	DWORD fmtSize; //						4B
//
//	WORD    wFormatTag;        /* format type */ //									2B
//	WORD    nChannels;         /* number of channels (i.e. mono, stereo, etc.) */ //2B
//	DWORD   nSamplesPerSec;    /* sample rate */ //									4B
//	DWORD   nAvgBytesPerSec;   /* for buffer estimation */ //						4B
//	WORD    nBlockAlign;       /* block size of data */ //							2B
//	WORD    wBitsPerSample; //														2B
//	char dataID[4];	//	"data" //													4B
//	DWORD dataSize; //																4B
//}WaveHeader;


class WaveRecorder : public QObject
{
	Q_OBJECT
	
public:
	WaveRecorder(const QString &fileName, const QAudioFormat &format, QObject *parent = nullptr);
	~WaveRecorder();
	void cacheWave(const QByteArray &raw);
	void cacheWave(const char *data, const quint32 len);
	void saveWave();
	void setRecordFileName(QString name);
	void clearWave();
private:
	QAudioFormat m_format;
	QString m_fileName;
	//WaveHeader wh;
	WavFile wavFile;
	bool isSaved;
	char *m_buffer;
	quint32 m_curPos;	// bufferµ±«∞÷∏’Î
};


#endif // !WAVEFILE_H