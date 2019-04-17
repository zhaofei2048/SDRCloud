#include "rtldevice.h"
#include "rtl-sdr.h"
#include <QDebug>
#include <QTimer>
#include <QThread>
#include "readrtlthread.h"

// 静态变量初始化
QReadWriteLock RTLDevice::buf16RWLock;
quint32 RTLDevice::buf16Len = 0;
qint16  RTLDevice::buf16[MAXIMUM_BUF_LENGTH];
QWaitCondition RTLDevice::buf16NotFull;
QWaitCondition RTLDevice::buf16NotEmpty;
QMutex RTLDevice::buf16Mutex;
bool RTLDevice::buf16Full = false;
RTLDevice::RTLDevice(QQueue<qreal> *queueBufX, QQueue<qreal> *queueBufY, QVector<qreal> *bufX, QVector<qreal> *bufY, QObject *parent)
	: SDRDevice(queueBufX, queueBufY, parent),
	m_bufX(bufX),
	m_bufY(bufY)
{
	// 使用计时器来触发updBuf的执行
	//updBufTimer = new QTimer;
	//connect(updBufTimer, SIGNAL(timeout()), this, SLOT(updateBuf()));

	// 初始化数据读取线程
	workerThread = new QThread;
	this->moveToThread(workerThread);
	connect(workerThread, &QThread::finished, this, &QObject::deleteLater);
	connect(this, SIGNAL(updBufSignal()), this, SLOT(updateBuf()));
	
}

RTLDevice::~RTLDevice()
{
	workerThread->quit();
	workerThread->wait();
}

quint32 RTLDevice::getDeviceCount()
{
	return quint32(rtlsdr_get_device_count());
}

QString RTLDevice::getDeviceName(quint32 index)
{
	int ind;
	ind = (uint32_t)index;
	/*qDebug() << "我是RTL的getDeviceName，我被执行了" << ind;*/
	return QString(rtlsdr_get_device_name(ind));
}

/*!
* 返回相应设备的USB信息
* \param manufact：制造商名（可能为NULL）
* \param product: USB设备型号名（可能为NULL）
* \param serial: USB设备序列号（可能为NULL）
* \return true on success
*/
bool RTLDevice::getDeviceUSBInfo(quint32 index, QString & manufact, QString & product, QString & serial)
{
	int r = 1;
	char strm[256];
	char strp[256];
	char strs[256];
	
	r = rtlsdr_get_device_usb_strings((uint32_t)index, strm, strp, strs);
	if (r == 0)
	{
		manufact = QString(strm);
		product = QString(strp);
		serial = QString(strs);
		return true;
	}
	return false;
}

bool RTLDevice::open(quint32 index)
{
	int r = 1;

	if (m_state == CLOSED)
	{
		r = rtlsdr_open((rtlsdr_dev_t**)&m_dev, (uint32_t)index);
		if (r == 0)
		{
			m_state = OPENED;
			return true;
		}
	}
	return false;
}

bool RTLDevice::close()
{
	int r = -1;
	if (m_state != CLOSED)
	{
		rtlsdr_cancel_async((rtlsdr_dev_t*)m_dev);	// 取消异步数据的读取
		r = rtlsdr_close((rtlsdr_dev_t*)m_dev);
		if (r == 0)
		{
			m_state = CLOSED;
		}
	}

	return r == 0 ? true : false;	// 无效（空）的设备会导致操作失败
}
// 此函数仅用于异步数据读取的回调
static void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx)
{
	qDebug() << "call back is running";
	RTLDevice::buf16Mutex.lock();	// 这里加条件变量的目的是为了防止读者独占读写锁的现象
	if (RTLDevice::buf16Full == true)
	{
		RTLDevice::buf16NotFull.wait(&RTLDevice::buf16Mutex);
	}
	RTLDevice::buf16Mutex.unlock();

	//RTLDevice::buf16RWLock.lockForWrite();
	for (int i = 0; i < len; i++)
	{
		RTLDevice::buf16[i] = ((qint16)buf[i] - 127);
	}
	RTLDevice::buf16Len = len;
	//RTLDevice::buf16RWLock.unlock();

	RTLDevice::buf16Mutex.lock();
	RTLDevice::buf16Full = true;
	RTLDevice::buf16NotEmpty.wakeAll();
	RTLDevice::buf16Mutex.unlock();

}
bool RTLDevice::readAsyncData(quint16 bufNum, quint32 bufLen)
{
	rtlsdr_reset_buffer((rtlsdr_dev_t *)m_dev);
	// 也许这里应该等待一些时间，给予buffer足够的复位时间
	// 这个rtlsdr_read_async函数是阻塞的，需要放到另一个线程

	/*r = rtlsdr_read_async((rtlsdr_dev_t *)m_dev,
		rtlsdr_callback,
		nullptr,
		(uint32_t)bufNum ,
		(uint32_t)bufLen);*/

	// 启动数据读取worker线程

	// 这里应该在加上一些其他条件才能开启这个计时器
	//updBufTimer->start(100);
	
	ReadRTLThread *rdThread = new ReadRTLThread(this);
	rdThread->start();
	qDebug() << "start read data thread successfuly";

	
	workerThread->start();
	emit updBufSignal();
	qDebug() << "start the worker thread successfuly";
	return true;
}

void RTLDevice::updateBuf()
{
	qint64 range = 2000;
	int resolution = 16;
	quint32 dataLen = 0;
	while (true)
	{
		RTLDevice::buf16Mutex.lock();
		if (buf16Full == false)
		{
			RTLDevice::buf16NotEmpty.wait(&RTLDevice::buf16Mutex);
		}
		RTLDevice::buf16Mutex.unlock();

		qDebug() << "updateBuf ready to run";
		//RTLDevice::buf16RWLock.lockForRead();
		qDebug() << "updateBuf is running";
		dataLen = buf16Len;
		for (int i = 0; i < dataLen / resolution; i++)
		{
			Q_ASSERT(resolution * i < 262144);
			m_QueueBufY->enqueue((qreal)(buf16[resolution * i]));
		}
		//RTLDevice::buf16RWLock.unlock();

		RTLDevice::buf16Mutex.lock();
		RTLDevice::buf16Full = false;
		RTLDevice::buf16NotFull.wakeAll();
		RTLDevice::buf16Mutex.unlock();

		for (int i = m_QueueBufY->count(); i > range; i--)
		{
			if (!m_QueueBufY->empty())
				m_QueueBufY->dequeue();
		}

		// 构造bufX的数据：1:count2
		int count2 = m_QueueBufY->count();
		while (!m_QueueBufX->empty())m_QueueBufX->dequeue();
		for (int i = 0; i < count2; i++)
		{
			m_QueueBufX->enqueue(i);
		}
		*m_bufX = m_QueueBufX->toVector();
		*m_bufY = m_QueueBufY->toVector();
	}
}

void RTLDevice::readAsyncDataSlot(quint16 bufNum, quint32 bufLen)
{
	int r = -1;
	r = rtlsdr_read_async((rtlsdr_dev_t *)m_dev,
	rtlsdr_callback,
	nullptr,
	(uint32_t)bufNum ,
	(uint32_t)bufLen);
}