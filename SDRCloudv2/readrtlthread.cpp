#include "readrtlthread.h"
#include "rtldevice.h"
ReadRTLThread::ReadRTLThread(RTLDevice *dongle):
m_dongle(dongle)
{
}

ReadRTLThread::~ReadRTLThread()
{
}

void ReadRTLThread::run()
{
	m_dongle->readAsyncDataSlot(0, 0);
}
