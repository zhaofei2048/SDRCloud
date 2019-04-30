#include "readdataworker.h"
#include "rtldevice.h"
#include <QDebug>
void ReadDataWorker::doWork(RtlDevice *dev)
{
	qDebug() << "start doing work(read data)";
	dev->m_readData();
}