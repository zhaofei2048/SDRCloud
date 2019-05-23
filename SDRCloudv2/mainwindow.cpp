#include "mainwindow.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QMessageBox>
//#include "qcustomplot.h"
#include "figure.h"
#include "rtldevice.h"
#include "dialogopendev.h"
#include <QTimer>
#include <QToolBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QProgressBar>
#include "demodulator.h"
#include "soundplayer.h"
//#include "imageviewer.h"

const QString MainWindow::demodMethodNames[8] = { "NFM", "WFM","AM","DSB","USB","LSB" ,"RAW","CW" };
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	m_recordTime(0),
	m_currentRecordTime(0)
	//imgViewer(nullptr)
{
	ui.setupUi(this);
	setWindowTitle(tr("SDRCloud"));
	
	m_dongle = new RtlDevice(nullptr);
	m_demod = new Demodulator(m_dongle);
	m_player = new SoundPlayer(m_demod);	// 初始化音频播放

	iniUI();	// 代码初始化UI

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
	/*initFigure();*/
	figure = new Figure(m_demod);
	setCentralWidget(figure);

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
	iniStatusBar();

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
	// 关联打开图像窗口
	connect(ui.actOpenImgWindow, SIGNAL(triggered(bool)), this, SLOT(openImgWindow(bool)));

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
					labDeviceName->setText(QString("设备:%0").arg(m_dongle->getDeviceName()));
					displayDefaultConfig();
				}
				else
				{
					console(QString("设备打开失败"));
				}
			}

		}
		// 清理对话框
		delete dialogDev;
	}	// !else m_dongle
}

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
	if (m_dongle->getState() == RtlDevice::RUNNING)
	{
		stopRtl();
		btnStartPlay->setText("开始播放");
	}
	if (m_dongle->close())
	{
		labDeviceName->setText(tr("设备:未打开"));
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

	// 1.创建声音录制相关UI
	QGroupBox *groupRecordUIs = new QGroupBox;
	comboxRecord = new QComboBox;
	btnClearRecord = new QPushButton;
	QHBoxLayout *recordHLayout = new QHBoxLayout;
	recordHLayout->addWidget(comboxRecord);
	recordHLayout->addWidget(btnClearRecord);
	QVBoxLayout *recordLayout = new QVBoxLayout;
	pgbRecord = new QProgressBar;
	recordLayout->addLayout(recordHLayout);
	recordLayout->addWidget(pgbRecord);
	recordLayout->addStretch();
	groupRecordUIs->setLayout(recordLayout);
	leftToolBox->addItem(groupRecordUIs, tr("录制设置"));


	// 2.创建解调方式选择的单选按钮组
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

	// 3.创建tuner设置的相关按钮
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

	// 4.创建rtl2832u设置的相关按钮
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

void MainWindow::iniStatusBar()
{
	labDeviceName = new QLabel(tr("设备:未打开"));
	statusBar()->addWidget(labDeviceName);
}
MainWindow::~MainWindow()
{
	//if (m_logFile != nullptr)
	//{
	//	fclose(m_logFile);
	//	m_logFile = nullptr;
	//}
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
	bool isOffsetTuningOn = m_dongle->isOffsetTuningOn();
	chkboxOffsetTuning->setChecked(isOffsetTuningOn);

	// 设置tuner和Rtl的AGC初始默认状态
	bool isTunerAgcOn = m_dongle->isTunerAgcOn();
	chkboxTunerAGC->setChecked(isTunerAgcOn);
	bool isRtlAgcOn = m_dongle->isRtlAgcOn();
	chkboxRtlAGC->setChecked(isRtlAgcOn);

	// 设置tuner增益范围条
	quint32 minGain = 0;
	quint32 maxGain = 0;
	m_dongle->getTunerGainRange(minGain, maxGain);
	slideRFGain->setRange(minGain, maxGain);
	quint32 tunerGain = m_dongle->getTunerGain();
	slideRFGain->setValue(tunerGain);

}

void MainWindow::startRtl()
{
	qDebug() << "start receive data";
	m_dongle->startReadData();	// 开始从硬件中读取数据，解调器会自动开始工作
	m_demod->startDemodFM();	// 注意：这里的调用顺序不能够更改，redraw和play操作依赖于running的demod，如果在start redraw和start play之前
								// 看到demod不是running的，那开启redraw和play线程的信号就会被浪费掉
	figure->startRedrawing();
	m_player->startPlay();
}

void MainWindow::stopRtl()
{
	m_player->stopPlay();
	figure->stopRedrawing();
	m_dongle->stopReadData();
	m_demod->stopDemodFM();
}

void MainWindow::btnStartPlaySlot()
{
	if (m_dongle->getState() == RtlDevice::RUNNING)
	{	
		stopRtl();
		btnStartPlay->setText("开始播放");
	}
	else
	{
		startRtl();
		btnStartPlay->setText("停止播放");
	}
}

void MainWindow::lostDevice(QString info)
{
	console("设备丢失（请关闭后重新插拔一下电视棒）："+info);
}

void MainWindow::openImgWindow(bool b)
{
	//if (imgViewer == nullptr) {
	//	imgViewer = new ImageViewer();
	//}
	//imgViewer->setWindowTitle(tr("NOAA卫星云图处理"));
	//imgViewer->show();
	;

}
