#include "readdataworker.h"
#include "rtldevice2.h"
#include <QDebug>
void ReadDataWorker::doWork(RtlDevice *dev)
{
	qDebug() << "start doing work(read data)";
	dev->m_readData();
}