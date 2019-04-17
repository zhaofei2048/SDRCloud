#include "sdrdevice.h"


SDRDevice::SDRDevice(QObject *parent)
	: QObject(parent),
	timerPlot(new QTimer())
{
}

SDRDevice::~SDRDevice()
{
}

QString SDRDevice::testRTL()
{
	int count = 0;
	int i = 0;
	QString info;

	count = rtlsdr_get_device_count();

	info += "count:" + QString::number(count) +"\n";
	for (i = 0; i < count; i++)
	{
		info += QString(rtlsdr_get_device_name(i)) + "\n";
	}
	return info;
}


static void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx)
{
	
}