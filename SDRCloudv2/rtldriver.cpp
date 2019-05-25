#include "rtldriver.h"
#include "rtl-sdr.h"
#include <QVector>
#include <QDebug>

static RTLDev dev;

RTLDriver::RTLDriver(QObject *parent)
	: QObject(parent)
{
}

RTLDriver::~RTLDriver()
{
}

quint32 RTLDriver::getDeviceCount()
{
	quint32 r = 0;

	r = (quint32)rtlsdr_get_device_count();
	return r;
}

QString RTLDriver::getDeviceName(const quint32 index)
{
	QString deviceName;
	
	deviceName = (QString)rtlsdr_get_device_name((uint32_t)index);

	return deviceName;
}

bool RTLDriver::getDeviceUSBStrings(const quint32 index, QString & manufact, QString & product, QString & serial)
{
	qint32 r = -1;
	char strM[256];
	char strP[256];
	char strS[256];

	r = rtlsdr_get_device_usb_strings((uint32_t)index, strM, strP, strS);

	if (r != 0)
	{
		manufact = "";
		product = "";
		serial = "";
		return false;
	}
	else
	{
		manufact = (QString)strM;
		product = (QString)strP;
		serial = (QString)strS;
		return true;
	}

	
}

bool RTLDriver::open(const quint32 index)
{
	qint32 r = -1;

	r = (qint32)rtlsdr_open(&dev, (uint32_t)index);

	return r == 0 ? true: false;
}

bool RTLDriver::close()
{
	qint32 r = -1;

	r = (qint32)rtlsdr_close(dev);

	return r == 0 ? true : false;
}

bool RTLDriver::setXtalFreq(const quint32 rtlFreq, const quint32 tunerFreq)
{
	qint32 r = -1;

	r = (qint32)rtlsdr_set_xtal_freq(dev, (uint32_t)rtlFreq, (uint32_t)tunerFreq);

	return r == 0 ? true : false;
}

bool RTLDriver::getXtalFreq(quint32 & rtlFreq, quint32 & tunerFreq)
{
	
	qint32 r = -1;
	uint32_t rfreq = 0;
	uint32_t tfreq = 0;

	r = rtlsdr_get_xtal_freq(dev, &rfreq, &tfreq);
	if (r != 0)
	{
		rtlFreq = 0;
		tunerFreq = 0;
		return false;
	}
	else
	{
		rtlFreq = (quint32)rfreq;
		tunerFreq = (quint32)tfreq;
		return true;
	}
}

bool RTLDriver::setCenterFreq(const quint32 freq)
{
	qint32 r = -1;

	r = rtlsdr_set_center_freq(dev, freq);
	qDebug() << "center freq changed" << freq << ":" << r;
	return r == 0 ? true : false;
}

quint32 RTLDriver::getCenterFreq()
{
	quint32 r = 0;

	r = (quint32)rtlsdr_get_center_freq(dev);

	return r;
}

bool RTLDriver::setFreqCorrection(qint32 ppm)
{
	qint32 r = -1;

	r = rtlsdr_set_freq_correction(dev, (int)ppm);

	return r == 0 ? true : false;
}

qint32 RTLDriver::getFreqCorrection()
{
	qint32 r = 0;

	r = (qint32)rtlsdr_get_freq_correction(dev);

	return r;
}

RTLDriver::TunerType RTLDriver::getTunerType()
{
	TunerType r = Unknown;

	r = (TunerType)rtlsdr_get_tuner_type(dev);

	return r;
}

quint32 RTLDriver::getTunerGains(QVector<qint32>& gains)
{
	qint32 count = -1;

	count = rtlsdr_get_tuner_gains(dev, NULL);

	if (count <= 0)return false;
	// 清空这个gains列表
	if (gains.isEmpty() == false)gains.resize(0);

	int *gainArray = new int[count];
	count = rtlsdr_get_tuner_gains(dev, gainArray);
	for (int i = 0; i < count; i++)
	{
		gains.append((qint32)gainArray[i]);
	}
	return count;
}

bool RTLDriver::setTuerGain(const qint32 gain)
{
	int r = -1;

	r = rtlsdr_set_tuner_gain(dev, gain);

	return r == 0 ? true : false;
}

qint32 RTLDriver::getTunerGain()
{
	qint32 r = 0;

	r = (qint32)rtlsdr_get_tuner_gain(dev);

	return r;
}
//
//quint32 RTLDriver::getTunerBandWidth()
//{
//	quint32 bw = 0;
//	int r = -1;
//
//	r = rtlsdr_set_and_get_tuner_bandwidth(dev, 0, &(uint32_t)bw, 0);
//
//	if (r != 0)return 0;	// error
//	return bw;
//}
//
//bool RTLDriver::setTunerBandWidth(const quint32 bw)
//{
//	int r = -1;
//
//	r = rtlsdr_set_tuner_bandwidth(dev, (uint32_t)bw);
//
//	return r == 0 ? true : false;
//}

bool RTLDriver::setTunerGainMode(const GainMode mode)
{
	int r = -1;

	r = rtlsdr_set_tuner_gain_mode(dev, mode);

	return r == 0 ? true : false;
}

bool RTLDriver::setTestMode(const bool on)
{
	int r = -1;

	r = rtlsdr_set_testmode(dev, on);
	
	return r == 0 ? true : false;
}

bool RTLDriver::setAgcMode(const bool on)
{
	int r = -1;

	r = rtlsdr_set_agc_mode(dev, on);

	return r == 0 ? true : false;
}

bool RTLDriver::setSampleRate(quint32 rate)
{
	int r = -1;

	r = rtlsdr_set_sample_rate(dev, (uint32_t)rate);

	return r == 0 ? true : false;
}

quint32 RTLDriver::getSampleRate()
{
	quint32 rate = 0;

	rate = (quint32)rtlsdr_get_sample_rate(dev);

	return rate;
}

bool RTLDriver::setSampleMode(const qint8 on)
{
	int r = -1;
	// on = 0: disabled; on = 1: IADC enabled; on = 2: QADC enabled;
	r = rtlsdr_set_direct_sampling(dev, on);

	return r == 0 ? true : false;
}

qint8 RTLDriver::getSampleMode()
{
	return qint8(rtlsdr_get_direct_sampling(dev));
}

bool RTLDriver::setOffsetTuning(const bool on)
{
	int r = -1;
	
	r = rtlsdr_set_offset_tuning(dev, on);

	return r == 0 ? true : false;
}

qint8 RTLDriver::getOffsetTuning()
{
	qint8 offsetTuningMode = -1;

	offsetTuningMode = rtlsdr_get_offset_tuning(dev);

	return offsetTuningMode;
}

bool RTLDriver::resetBuffer()
{
	int r = -1;

	r = rtlsdr_reset_buffer(dev);

	return r == 0 ? true : false;
}

bool RTLDriver::readAsync(rtlsdr_read_async_cb_t cb, void * ctx, quint32 bufNum, quint32 bufLen)
{
	int r = -1;

	r = rtlsdr_read_async(dev, cb, ctx, (uint32_t)bufNum, (uint32_t)bufLen);
	return r == 0 ? true : false;
}

bool RTLDriver::cancelAsync()
{
	int r = -1;

	r = rtlsdr_cancel_async(dev);

	return r == 0 ? true : false;
}
