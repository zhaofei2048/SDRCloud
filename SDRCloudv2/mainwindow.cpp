#include "mainwindow.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QMessageBox>
#include "qcustomplot.h"
#include "rtldevice2.h"
#include "dialogopendev.h"
#include <QTimer>
#include <QToolBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>


//const QString MainWindow::demodMethodNames[8] = { tr("NFM"), tr("WFM"),tr("AM"),tr("DSB"),tr("USB"),tr("LSB") ,tr("RAW"),tr("CW") };
const QString MainWindow::demodMethodNames[8] = { "NFM", "WFM","AM","DSB","USB","LSB" ,"RAW","CW" };
MainWindow::MainWindow(FILE *logFile, QWidget *parent)
	: QMainWindow(parent),
	m_logFile(logFile),
	updateFigureBufWorker(nullptr)
{
	ui.setupUi(this);
	setWindowTitle(tr("SDRCloud"));
	iniUI();	// 代码初始化UI
	

	// 显式初始化成员变量
	//m_QueueBufX = new QQueue<qreal>;
	//m_QueueBufY = new QQueue<qreal>;
	//m_bufX = new QVector<qreal>;
	//m_bufY = new QVector<qreal>;
	//m_dongle = new RTLDevice(m_QueueBufX, m_QueueBufY, m_bufX, m_bufY, 0);
	//updWaveTimer = new QTimer;
	m_dongle = new RtlDevice(nullptr);


	connectSignalSlot();	// 代码连接信号和槽
	
}

void MainWindow::iniUI()
{
	// 初始化主窗口
	resize(1280, 840);
	setWindowTitle(tr("SDRCloud"));
	//// 实例化客户区布局容器
	//div = new QWidget;	// 暂时不启用，如果需要在客户去进行复杂布局再启用
	//setCentralWidget(div);
	
	// 添加customplot组件到客户区
	initFigure();
	

	// 实例化下方控制台浮动窗口
	dockConsole = new QDockWidget(tr("消息"));
	dockConsole->setAllowedAreas(Qt::BottomDockWidgetArea);
	//dockConsole->setFeatures(QDockWidget::NoDockWidgetFeatures);
	// 实例化控制台组件
	txtConsole = new QTextEdit;
	txtConsole->setMinimumSize(300, 100);
	dockConsole->setWidget(txtConsole);

	// 实例化瀑布图浮动窗口
	dockWaterfall = new QDockWidget;
	dockWaterfall->setAllowedAreas(Qt::BottomDockWidgetArea);
	dockWaterfall->setFeatures(QDockWidget::NoDockWidgetFeatures);

	// 实例化左侧浮动窗口
	dockLeft = new QDockWidget;
	dockLeft->setAllowedAreas(Qt::LeftDockWidgetArea);
	dockLeft->setFeatures(QDockWidget::NoDockWidgetFeatures);
	
	// 放置DockWidget
	addDockWidget(Qt::LeftDockWidgetArea, dockLeft, Qt::Vertical);
	addDockWidget(Qt::BottomDockWidgetArea, dockWaterfall, Qt::Vertical);	// 将两个Dock垂直放置
	addDockWidget(Qt::BottomDockWidgetArea, dockConsole, Qt::Vertical);

	iniLeftToolBoxUI();
}

void MainWindow::connectSignalSlot()
{
	// 连接RTL->打开RTL设备action和其槽函数
	connect(ui.actOpenRTL, SIGNAL(triggered(bool)), this, SLOT(openRTL(bool)));
	// 连接计时器更新figure波形图
	//connect(updWaveTimer, SIGNAL(timeout()), this, SLOT(updateWave()));
	// 连接关闭RTL设备的action和槽函数
	connect(ui.actCloseRTL, SIGNAL(triggered(bool)), this, SLOT(closeRTL(bool)));
	// 关联menuRTL的显示槽函数
	connect(ui.menuRTL, SIGNAL(aboutToShow()), this, SLOT(showMenuRTL()));
	// 关联开始播放与在硬件上读取数据
	connect(btnStartPlay, SIGNAL(clicked()), this, SLOT(btnStartPlaySlot()));
}

void MainWindow::openRTL(bool b)
{
	Q_UNUSED(b);
	if (m_dongle->getState() != RtlDevice::CLOSED)
	{
		/*qDebug() << "dongle state=" << m_dongle->getState();*/
		QMessageBox::warning(this, tr("警告"), tr("当前已经存在一个打开的RTL设备，请先关闭它再尝试打开"));
	}	// !if (m_dongle
	else
	{
		quint32 index = 0;
		int ret = QDialog::Rejected;
		QListWidgetItem *item = nullptr;
		DialogOpenDev *dialogDev = new DialogOpenDev(m_dongle, this);
		
		ret = dialogDev->exec();
		if (ret == QDialog::Accepted)
		{
			item = dialogDev->getSelectedItem();
			if (item != nullptr)
			{	// 正常情况下item不会是空的
				index = (quint32)item->data(Qt::UserRole).toInt();
				QMessageBox::information(this, tr("成功"), QString::asprintf("你打开的设备索引是：%d", index));
				// todo:在这里实际打开RTL设备，有可能成功也可能失败
				// 设备打开成功后应该在这里将设备状态更改，否则会重复打开
				if (m_dongle->open(index))
				{
					displayDefaultConfig();
				}
				else
				{
					console(QString("设备打开失败"));
				}
				/*if (m_dongle->open(index))
				{
					console(QString("设备成功打开"));
					if (m_dongle->readAsyncData())
					{
						console(QString("设备开始异步读取数据"));
						updWaveTimer->start(100);
					}
					else
					{
						console(QString("设备异步读取数据失败"));
					}
					
				}
				else
				{
					console(QString("设备打开失败"));
				}*/
			}

		}
		// 清理对话框
		delete dialogDev;
	}	// !else m_dongle
}

//void MainWindow::updateWave()
//{
//	//QVector<qreal> x(101);
//	//QVector<qreal> y(100);
//	//for (int i = 0; i < 100; i++)
//	//{
//	//	x.append(i);
//	//	y.append(50);
//	//}
//	//x.append(100);
//	/*figure->graph(0)->setData(m_QueueBufX->toVector(), m_QueueBufY->toVector(), true);*/
//	/*figure->graph(0)->setData(x, y, true);*/
//
//	//figure->graph(0)->setData(*m_bufX, *m_bufY, true);
//	//figure->replot();
//	
//}

void MainWindow::console(const QString & info)
{
	txtConsole->append(info + "\n");
}

void MainWindow::showMenuRTL()
{
	if (m_dongle->getState() == RtlDevice::CLOSED)
	{
		ui.actCloseRTL->setEnabled(false);
	}
	else
	{
		ui.actCloseRTL->setEnabled(true);
	}
}

void MainWindow::closeRTL(bool b)
{
	Q_UNUSED(b);
	if (m_dongle->close())
	{
		console(QString("成功关闭RTL设备"));
	}
	else
	{
		console(QString("关闭RTL设备失败"));
	}
}

void MainWindow::iniLeftToolBoxUI()
{
	// 对左边工具区进行初始化并布局================================================================
	// 创建开始播放的按钮
	btnStartPlay = new QPushButton(tr("开始播放"));
	btnStartPlay->setStyleSheet(
		//正常状态样式
		"QPushButton{"
		"background-color:rgba(100,225,100,30);"//背景色（也可以设置图片）
		"border-style:outset;"                  //边框样式（inset/outset）
		"border-width:4px;"                     //边框宽度像素
		"border-radius:10px;"                   //边框圆角半径像素
		"border-color:rgba(150,150,150,200);"    //边框颜色
		"font:bold 10px;"                       //字体，字体大小
		"color:rgba(0,0,0,100);"                //字体颜色
		"padding:6px;"                          //填衬
		"}"
		//鼠标按下样式
		"QPushButton:pressed{"
		"background-color:rgba(100,255,100,200);"
		"border-color:rgba(255,255,255,30);"
		"border-style:inset;"
		"color:rgba(0,0,0,100);"
		"}"
		//鼠标悬停样式
		"QPushButton:hover{"
		"background-color:rgba(100,255,100,100);"
		"border-color:rgba(255,255,255,200);"
		"color:rgba(0,0,0,200);"
		"}");
	dockLeft->setTitleBarWidget(btnStartPlay);
	leftToolBox = new QToolBox;
	// 1.创建解调方式选择的单选按钮组
	QHBoxLayout *demodLayout = new QHBoxLayout;
	// 先创建四个列布局
	QVBoxLayout *demodLayoutc[4];
	for (int i = 0; i < 4; i++)
	{
		demodLayoutc[i] = new QVBoxLayout;
	}
	QGroupBox *groupDemodBtns = new QGroupBox;

	for (int i = 0; i < 8; i++)
	{
		rbtnDemodMode[i] = new QRadioButton(MainWindow::demodMethodNames[i]);


		demodLayoutc[qFloor(i / 2.0)]->addWidget(rbtnDemodMode[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		demodLayoutc[i]->addStretch();
		demodLayout->addLayout(demodLayoutc[i]);
	}
	demodLayout->addStretch();
	groupDemodBtns->setLayout(demodLayout);

	leftToolBox->setMinimumWidth(250);
	leftToolBox->addItem(groupDemodBtns, tr("解调方法"));

	// 2.创建tuner设置的相关按钮
	QGroupBox *groupTunerSetting = new QGroupBox;
	QVBoxLayout *tunerSettingLayout = new QVBoxLayout;
	QHBoxLayout *tsLayout1 = new QHBoxLayout;
	QLabel *labTunerFreq = new QLabel(tr("RF频率:"));
	sboxTunerFreq = new QDoubleSpinBox;
	tsLayout1->addWidget(labTunerFreq);
	tsLayout1->addWidget(sboxTunerFreq);

	QLabel *labRFGain = new QLabel(tr("RF增益"));
	slideRFGain = new QSlider(Qt::Horizontal);

	QLabel *labIFGain = new QLabel(tr("IF增益"));
	slideIFGain = new QSlider(Qt::Horizontal);

	chkboxOffsetTuning = new QCheckBox(tr("Offset Tuning"));
	chkboxOffsetTuning->setLayoutDirection(Qt::RightToLeft);
	// 添加对AGC的开关
	QGroupBox *groupAGCSetting = new QGroupBox(tr("AGC设置"));
	QHBoxLayout *agcSettingLayout = new QHBoxLayout;
	chkboxTunerAGC = new QCheckBox(tr("Tuner AGC"));
	chkboxTunerAGC->setLayoutDirection(Qt::RightToLeft);
	chkboxRtlAGC = new QCheckBox(tr("RTL AGC"));
	chkboxRtlAGC->setLayoutDirection(Qt::RightToLeft);
	agcSettingLayout->addWidget(chkboxTunerAGC);
	agcSettingLayout->addWidget(chkboxRtlAGC);
	groupAGCSetting->setLayout(agcSettingLayout);

	tunerSettingLayout->addLayout(tsLayout1);
	tunerSettingLayout->addWidget(labRFGain);
	tunerSettingLayout->addWidget(slideRFGain);
	tunerSettingLayout->addWidget(labIFGain);
	tunerSettingLayout->addWidget(slideIFGain);
	tunerSettingLayout->addWidget(groupAGCSetting);
	tunerSettingLayout->addWidget(chkboxOffsetTuning, 0, Qt::AlignHCenter);
	tunerSettingLayout->addStretch();

	groupTunerSetting->setLayout(tunerSettingLayout);
	leftToolBox->addItem(groupTunerSetting, tr("tuner设置"));

	// 2.创建rtl2832u设置的相关按钮
	QGroupBox *groupRtlSetting = new QGroupBox;
	QVBoxLayout *rtlSettingLayout = new QVBoxLayout;

	QGroupBox *groupSampleRateContainer = new QGroupBox(tr("采样率设置"));
	QVBoxLayout *sampleRateLayout = new QVBoxLayout;
	comboxSampleRate = new QComboBox;
	sampleRateLayout->addWidget(comboxSampleRate);
	groupSampleRateContainer->setLayout(sampleRateLayout);

	QGroupBox *groupSampleModeContainer = new QGroupBox(tr("采样模式设置"));
	QVBoxLayout *sampleModeLayout = new QVBoxLayout;
	comboxSampleMode = new QComboBox;
	sampleModeLayout->addWidget(comboxSampleMode);
	groupSampleModeContainer->setLayout(sampleModeLayout);

	rtlSettingLayout->addWidget(groupSampleRateContainer);
	rtlSettingLayout->addWidget(groupSampleModeContainer);
	rtlSettingLayout->addStretch();

	groupRtlSetting->setLayout(rtlSettingLayout);
	leftToolBox->addItem(groupRtlSetting, tr("采样设置"));
	// todo:还需要添加对tuner以及rtl2832u的AGC模式的开关按钮

	dockLeft->setWidget(leftToolBox);
	// 对左边工具区布局结束================================================================

}

MainWindow::~MainWindow()
{
	if (m_logFile != nullptr)
	{
		fclose(m_logFile);
		m_logFile = nullptr;
	}
	if (m_dongle->getState() != RtlDevice::CLOSED)
	{
		closeRTL(true);
	}
}

void MainWindow::displayDefaultConfig()
{
	QVector<quint32> sampleRateList;
	sampleRateList = m_dongle->getDefaultSampleRateList();

	// 在combobox中显示采样频率列表
	comboxSampleRate->clear();
	for (int i = 0; i < sampleRateList.count(); i++)
	{
		comboxSampleRate->addItem(QString("%0 MSPS").arg(sampleRateList[i] / 1000000.0), QVariant(sampleRateList[i]));
	}
	quint32 sampleRate = m_dongle->getSampleRateIndex();
	comboxSampleRate->setCurrentIndex(sampleRate);

	// 在combobox中设置显示采样模式列表
	comboxSampleMode->clear();
	for (int i = 0; i < 3; i++)
	{
		comboxSampleMode->addItem(RtlDevice::sampleModeStrings[i], QVariant(i));
	}
	quint32 sampleMode = m_dongle->getSampleMode();
	comboxSampleMode->setCurrentIndex(sampleMode);

	// 设置offset tuning复选框默认状态
	bool isOffsetTuningOn = m_dongle->getOffsetTuningState();
	chkboxOffsetTuning->setChecked(isOffsetTuningOn);

	// 设置tuner和Rtl的AGC初始默认状态
	bool isTunerAgcOn = m_dongle->getTunerAgcState();
	chkboxTunerAGC->setChecked(isTunerAgcOn);
	bool isRtlAgcOn = m_dongle->getRtlAgcState();
	chkboxRtlAGC->setChecked(isRtlAgcOn);

	// 设置tuner增益范围条
	quint32 minGain = 0;
	quint32 maxGain = 0;
	m_dongle->getTunerGainRange(minGain, maxGain);
	slideRFGain->setRange(minGain, maxGain);
	quint32 tunerGain = m_dongle->getTunerGain();
	slideRFGain->setValue(tunerGain);

}

void MainWindow::initFigure()
{
	m_figureBufLen = 2000;
	figure = new QCustomPlot;
	figure->addGraph();
	figure->xAxis->setLabel(tr("time"));
	figure->yAxis->setLabel(tr("signal"));
	figure->xAxis->setRange(0, m_figureBufLen);
	figure->yAxis->setRange(-1.0, 1.0);
	setCentralWidget(figure);
	
	// 更新figure buffer的线程
	if (updateFigureBufWorker == nullptr)
	{
		updateFigureBufWorker = new UpdateFigureBufWorker();
	}
	updateFigureBufWorker->moveToThread(&updateFigureBufThread);
	connect(this, &MainWindow::updateFigureBufSignal, updateFigureBufWorker, &UpdateFigureBufWorker::doWork);
	updateFigureBufThread.start();

	// 初始化更新绘图的timer
	updateFigureTimer = new QTimer();
	connect(updateFigureTimer, SIGNAL(timeout()), this, SLOT(updatePsdWave()));
}

void MainWindow::updateFigureBuf()
{
	QVector<qreal> data;
	quint32 step = 20;
	while (true)
	{
		if (m_dongle->getState() != RtlDevice::RUNNING)	// 如果硬件都没有在运行，那也就没有必要读取更新数据了
		{
			break;
		}
		m_dongle->getData(data);	// 此函数会被阻塞

		for (int i = 0; i < data.count(); i = i+step)
		{
			m_figureBufY.enqueue(data[i]);
		}

		for (int i = m_figureBufY.count(); i > m_figureBufLen; i--)
		{
			m_figureBufY.removeFirst();
		}

		if (m_figureBufX.isEmpty() || m_figureBufX.count() != m_figureBufY.count())
		{
			for (int i = m_figureBufX.count(); i < m_figureBufY.count(); i++)
			{
				m_figureBufX.enqueue(i);
			}
		}

		m_figureX = m_figureBufX.toVector();
		m_figureY = m_figureBufY.toVector();

	}
}

void MainWindow::updatePsdWave()
{
	emit updateFigureBufSignal(this);	// 这里会立即返回不会阻塞
	figure->graph(0)->setData(m_figureX, m_figureY, true);
	figure->replot();
}

void MainWindow::startReceiveData()
{
	qDebug() << "start receive data";
	m_dongle->startReadData();	// 开始从硬件中读取数据
	qDebug() << "start the timer for update the figure";
	if (updateFigureTimer->isActive() == false)
	{
		updateFigureTimer->start(0);	// 尽快开始更新figure
	}
}



void UpdateFigureBufWorker::doWork(MainWindow *w)
{
	qDebug() << "start doing work(update figure buffer)";
	w->updateFigureBuf();
}

void MainWindow::btnStartPlaySlot()
{
	if (m_dongle->getState() == RtlDevice::RUNNING)
	{	
		stopReceiveData();
	}
	else
	{
		startReceiveData();
	}
}

void MainWindow::stopReceiveData()
{
	updateFigureTimer->stop();
	m_dongle->stopReadData();
}
