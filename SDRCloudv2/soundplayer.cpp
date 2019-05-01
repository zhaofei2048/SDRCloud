#include "soundplayer.h"
#include "pcmiodevice.h"
#include <QDebug>
#include "constant.h"
#include "demodulator.h"

SoundPlayer::SoundPlayer(Demodulator *demod, QObject *parent)
	: QObject(parent),
	m_demod(demod)
{
	initSoundPlayer();
}

SoundPlayer::~SoundPlayer()
{
	updateAudioBufThread.quit();
	updateAudioBufThread.wait();
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
}

void SoundPlayer::destroy()
{
	m_audio->stop();
	delete m_audio;
}

void SoundPlayer::startUpdateAudioBuf()
{
	emit updateAudioBufSignal(this);
}

void SoundPlayer::initSoundPlayer()
{
	m_pcmIODevice = new PCMIODevice();
	// Set up the format, eg.
	format.setSampleRate(CONFIG_DEFAULT_AUDIO_RATE);
	format.setChannelCount(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

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
		if (m_demod->isRunning() == false)
		{
			break;
		}
		m_demod->getDataByChar(data, len);
		// 将数据写到audio buffer里
		m_pcmIODevice->writeData(data, len);
	}

}

void SoundPlayer::handleStateChanged(QAudio::State newState)
{
	switch (newState) {
	case QAudio::IdleState:
		// Finished playing (no more data)
		qDebug() << "audio play on idle";
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
		qDebug() << "player stopped";
		break;

	default:
		// ... other cases as appropriate
		break;
	}
}

void UpdateAudioBufWorker::doWork(SoundPlayer *player)
{
	qDebug() << "start doing work(update audio buffer)";
	player->m_updateAudioBuf();
}