#include "sdrcloud.h"

SDRCloud::SDRCloud(QWidget *parent)
	: QMainWindow(parent),

	rtl(new SDRDevice(this))
{	
	// 可视化设计部分UI初始化
	ui.setupUi(this);
	// 代码设计部分UI初始化
	qDebug() << "UI初始化";
	iniUI();
	
	
	// 设置音频输入
	QAudioFormat formatAudio;
	formatAudio.setSampleRate(8000);
	formatAudio.setChannelCount(1);
	formatAudio.setSampleSize(8);
	formatAudio.setCodec("audio/pcm");
	formatAudio.setByteOrder(QAudioFormat::LittleEndian);
	formatAudio.setSampleType(QAudioFormat::UnSignedInt);

	QAudioDeviceInfo inputDevices = QAudioDeviceInfo::defaultInputDevice();
	m_audioInput = new QAudioInput(inputDevices, formatAudio, this);
	m_audioInput->setNotifyInterval(20);
	m_QueueBuf = new QQueue<qreal>;
	m_QueueBufX = new QQueue<qreal>;
	m_audioIODevice = new AudioIODevice(this, m_vecSample, m_sampleX, m_QueueBuf, m_QueueBufX);
	m_audioIODevice->open(QIODevice::WriteOnly);
	myTimer = new QTimer;
	//connect(m_audioInput, SIGNAL(notify()), this, SLOT(updateWaveSlot()));
	connect(myTimer, SIGNAL(timeout()), this, SLOT(updateWaveSlot()));
	m_audioInput->start(m_audioIODevice);
	test();
}

SDRCloud::~SDRCloud()
{
	m_audioInput->stop();
	m_audioIODevice->close();
}

void SDRCloud::iniUI()
{	
	ploter = new QCustomPlot;
	txtEditConsole = new QPlainTextEdit(this);
	leftToolBox = new QToolBox;

	// 初始部分组件布局

	//ploter->setMinimumSize(800, 300);
	txtEditConsole->move(0, 300);
	txtEditConsole->setMinimumSize(800, 300);
	ploter->addGraph();
	ploter->xAxis->setLabel("time");
	ploter->yAxis->setLabel("volume");
	// set axes ranges, so we see all data:
	ploter->xAxis->setRange(0, 2000);
	ploter->yAxis->setRange(-1, 1);
	//// 工具箱和ploter水平布局
	//QHBoxLayout *hLay1 = new QHBoxLayout;
	//hLay1->addWidget(leftToolBox);
	//hLay1->addWidget(ploter);

	//// 前面的组件和控制台垂直布局
	//QVBoxLayout *vLay1 = new QVBoxLayout;
	//vLay1->addLayout(hLay1);
	//vLay1->addWidget(txtEditConsole);

}

void SDRCloud::printToConsole(const QString &s)
{
	txtEditConsole->appendPlainText(s);
}

void SDRCloud::test()
{

	printToConsole(rtl->testRTL());
}



void SDRCloud::updateWave(const QVector<qreal> data, quint16 len)
{

	//ploter.graph(0)->setData(bufX, buf);
	//ploter.xAxis->setLabel("x");
	//ploter.yAxis->setLabel("y");
	//// set axes ranges, so we see all data:
	//ploter.xAxis->setRange(0, len);
	//ploter.yAxis->setRange(0, maxY);
	//ploter.replot();
}
void SDRCloud::updateWaveSlot()
{
	//QVector<qreal> x;
	//QVector<qreal> y;
	//for (int i = 0; i < 2000; i++)
	//{
	//	x.append(i);
	//	y.append(100);
	//}
	//ploter->graph(0)->setData(x, y);
	ploter->graph(0)->setData(m_QueueBufX->toVector(), m_QueueBuf->toVector());
	ploter->replot();
}
