#include "soundplayer.h"
#include "pcmiodevice.h"
#include <QDebug>
#include "constant.h"
#include "demodulator.h"
#include "waverecorder.h"

SoundPlayer::SoundPlayer(Demodulator *demod, QObject *parent)
	: QObject(parent),
	m_demod(demod)	
{
	initSoundPlayer();	// 注意，这里要确保format被初始化好了，才能去初始化recorder
}

SoundPlayer::~SoundPlayer()
{
	updateAudioBufThread.quit();
	updateAudioBufThread.wait();
	// 用于防止程序意外终止，
	//m_waveRecorder->saveWave();	// 如果当前缓冲区已被存储，此函数不会发生作用，因此是安全的
	destroy();
}

void SoundPlayer::startPlay()
{
	startUpdateAudioBuf();
	m_audio->start(m_pcmIODevice);	// 启动声音播放
	qDebug() << "start play m_audio";
	
}

void SoundPlayer::stopPlay()
{
	m_audio->suspend();
	//WaveFile::saveWave("E:/workspace/Qt/SDRCloud/waves/wave01.wav", buf, format);
	m_waveRecorder->saveWave();
	//buf.clear();
	//m_pcmIODevice->stopUpdate();
}

//void SoundPlayer::saveSound()
//{
//	// 用于防止程序意外终止，
//	m_waveRecorder->saveWave();
//}

void SoundPlayer::destroy()
{
	m_audio->stop();
	delete m_audio;
	delete m_waveRecorder;
}

void SoundPlayer::startUpdateAudioBuf()
{
	emit updateAudioBufSignal(this);
}

void SoundPlayer::initSoundPlayer()
{
	format.setSampleRate(CONFIG_DEFAULT_AUDIO_RATE);
	format.setChannelCount(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);

	m_waveRecorder = new WaveRecorder("E:/workspace/Qt/SDRCloud/waves/wave02.wav", format);
	m_pcmIODevice = new PCMIODevice(m_waveRecorder);
	// Set up the format, eg.

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	if (!info.isFormatSupported(format)) {
		qWarning() << "Raw audio format not supported by backend, cannot play audio.";
		return;
	}

	m_audio = new QAudioOutput(format, this);
	connect(m_audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

	updateAudioBufWorker = new UpdateAudioBufWorker();
	updateAudioBufWorker->moveToThread(&updateAudioBufThread);
	connect(this, &SoundPlayer::updateAudioBufSignal, updateAudioBufWorker, &UpdateAudioBufWorker::doWork);
	updateAudioBufThread.start();
}

void SoundPlayer::m_updateAudioBuf()
{
	char data[DEFAULT_MAX_BUFFER_SIZE];
	quint32 len = 0;
	while (true)
	{
		//qDebug() << m_demod->isRunning();
		if (m_demod->isRunning() == false)
		{

			break;
		}
		m_demod->getDataByChar(data, len);
		// 将数据写到audio buffer里
		//buf.append(data, len);	// 只记录稳定时刻
		//WaveFile::saveWave("E:/workspace/Qt/SDRCloud/waves/wave01.wav", buf, format);
		//buf.clear();
		m_pcmIODevice->writeData(data, len);
	}
}

void SoundPlayer::handleStateChanged(QAudio::State newState)
{
	switch (newState) {
	case QAudio::IdleState:
		// Finished playing (no more data)
		qDebug() << "*********audio play on idle*********";
		//m_audio->stop();
		/*m_sourceFile.close();*/
		/*delete m_audio;*/
		break;

	case QAudio::StoppedState:
		// Stopped for other reasons
		if (m_audio->error() != QAudio::NoError) {
			// Error handling
			qDebug() << "Error on audio play";
		}
		qDebug() << "************player stopped***********";
		break;

	default:
		// ... other cases as appropriate
		break;
	}
}

void SoundPlayer::setRecordFileName(QString name)
{
	m_waveRecorder->setRecordFileName(name);
}

void UpdateAudioBufWorker::doWork(SoundPlayer *player)
{
	qDebug() << "start doing work(update audio buffer)";
	player->m_updateAudioBuf();
}

void SoundPlayer::setRecordState(bool on)
{
	m_pcmIODevice->setRecordState(on);
}

void SoundPlayer::clearRecord()
{
	m_waveRecorder->clearWave();
}