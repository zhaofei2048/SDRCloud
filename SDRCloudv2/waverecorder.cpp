#include "waverecorder.h"
#include <QFile>
#include <QDebug>


WaveRecorder::WaveRecorder(const QString &fileName, const QAudioFormat &format, QObject *parent)
	: QObject(parent),
	isSaved(true),
	m_buffer(new char[WAVE_FILE_BUFFER_SIZE]),
	m_curPos(0),
	m_fileName(fileName),
	m_format(format)
{
	//// 初始化时就初始化RIFF头
	//WaveHeader wh;
	//strcpy(wh.riffID, "RIFF");
	//wh.riffSize = 0 + 36;	// size = fileSize-8 = dataSize+44-8=dataSize+36
	//strcpy(wh.riffType, "WAVE");	// 表示后面需要两个区块：format and data chunk
	//strcpy(wh.fmtID, "fmt ");
	//wh.fmtSize = 16;
	//wh.wFormatTag = WAVE_FORMAT_PCM;
	//wh.nChannels = m_format.channelCount();
	//wh.nSamplesPerSec = m_format.sampleRate();
	//wh.wBitsPerSample = m_format.sampleSize();
	//wh.nBlockAlign = wh.nChannels*wh.wBitsPerSample / 8;
	//wh.nAvgBytesPerSec = wh.nBlockAlign*wh.nSamplesPerSec;

	//// sub data block
	//strcpy(wh.dataID, "data");
	//wh.dataSize = 0;

	//// 未来需要将riffSize和dataSize替换为实际的值
	//

}

WaveRecorder::~WaveRecorder()
{
	if (isSaved == false)
	{
		saveWave();
	}
	delete m_buffer;
}


void WaveRecorder::saveWave()
{
	//wh.riffSize += m_curPos;
	//wh.dataSize = m_curPos;
	//qDebug() << "recoring audio...";
	//QFile f(m_fileName);
	//f.open(QFile::WriteOnly);
	////char info[] = "hello world";
	////f.write(info);
	//f.write((char *)&wh, 44);	// 直接44B
	//qDebug() << QString::number(wh.dataSize);
	//f.write(m_buffer, wh.dataSize);
	//f.close();
	//m_curPos = 0;
	if (isSaved == false) {
		wavFile.writeWave(m_fileName, m_format, m_buffer, m_curPos);
		m_curPos = 0;
		isSaved = true;
	}
	//qDebug() << "wav header size:" << QString::number(sizeof(wh));
}

void WaveRecorder::setRecordFileName(QString name)
{
	m_fileName = name;
}


void WaveRecorder::cacheWave(const QByteArray &data)
{
	if (m_curPos + data.count() <= WAVE_FILE_BUFFER_SIZE)
	{
		memcpy(m_buffer + m_curPos, data.constData(), data.count());
		m_curPos += data.count();
		isSaved = false;
	}
}

void WaveRecorder::cacheWave(const char *data, const quint32 len)
{
	if ((m_curPos+len) < WAVE_FILE_BUFFER_SIZE)
	{	// 确保缓冲数组不要溢出
		// 注意strncpy这个函数复制发现src中的null会提前结束复制，即使没有到达要求的长度n
		memcpy(m_buffer + m_curPos, data, len);
		m_curPos += len;
		isSaved = false;
		//qDebug() << QString::number(len);
	}
}

void WaveRecorder::clearWave()
{
	wavFile.clearAndWriteWave(m_fileName, m_format, m_buffer, 0);
}

