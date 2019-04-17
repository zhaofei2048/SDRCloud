#include "mainwindow.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QMessageBox>
#include "qcustomplot.h"
#include "rtldevice.h"
#include "dialogopendev.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("SDRCloud"));
	iniUI();	// 代码初始化UI
	

	// 显式初始化成员变量
	m_QueueBufX = new QQueue<qreal>;
	m_QueueBufY = new QQueue<qreal>;
	m_bufX = new QVector<qreal>;
	m_bufY = new QVector<qreal>;
	m_dongle = new RTLDevice(m_QueueBufX, m_QueueBufY, m_bufX, m_bufY, 0);
	updWaveTimer = new QTimer;

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
	figure = new QCustomPlot;
	figure->addGraph();
	figure->xAxis->setLabel(tr("time"));
	figure->yAxis->setLabel(tr("signal"));
	figure->xAxis->setRange(0, 2000);
	figure->yAxis->setRange(-200, 200);
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
	
}

void MainWindow::connectSignalSlot()
{
	// 连接RTL->打开RTL设备action和其槽函数
	connect(ui.actOpenRTL, SIGNAL(triggered(bool)), this, SLOT(openRTL(bool)));
	// 连接计时器更新figure波形图
	connect(updWaveTimer, SIGNAL(timeout()), this, SLOT(updateWave()));
	// 连接关闭RTL设备的action和槽函数
	connect(ui.actCloseRTL, SIGNAL(triggered(bool)), this, SLOT(closeRTL(bool)));
	// 关联menuRTL的显示槽函数
	connect(ui.menuRTL, SIGNAL(aboutToShow()), this, SLOT(showMenuRTL()));
}

void MainWindow::openRTL(bool b)
{
	Q_UNUSED(b);
	if (m_dongle->getState() != SDRDevice::CLOSED)
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
				}
			}

		}
		// 清理对话框
		delete dialogDev;
	}	// !else m_dongle
}

void MainWindow::updateWave()
{
	//QVector<qreal> x(101);
	//QVector<qreal> y(100);
	//for (int i = 0; i < 100; i++)
	//{
	//	x.append(i);
	//	y.append(50);
	//}
	//x.append(100);
	/*figure->graph(0)->setData(m_QueueBufX->toVector(), m_QueueBufY->toVector(), true);*/
	/*figure->graph(0)->setData(x, y, true);*/
	figure->graph(0)->setData(*m_bufX, *m_bufY, true);
	figure->replot();
}

void MainWindow::console(const QString & info)
{
	txtConsole->append(info + "\n");
}

void MainWindow::showMenuRTL()
{
	if (m_dongle->getState() == SDRDevice::CLOSED)
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
