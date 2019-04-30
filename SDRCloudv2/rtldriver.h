#pragma once
/*
	设备驱动接口
*/
#ifndef RTLDRIVER_H
#define RTLDRIVER_H

#include <QObject>
typedef struct rtlsdr_dev *RTLDev;
// 此类主要是为了本地化rtl 原始的驱动接口
class RTLDriver : public QObject
{
	Q_OBJECT
public:
	typedef enum TunerType_flag
	{
		Unknown = 0,
		E4000,
		FC0012,
		FC0013,
		FC2580,
		R820T
	}TunerType;
	typedef enum GainMode_flag
	{
		Automatic = 0,
		Manual
	
	}GainMode;
	// rtl read async data回调函数类型
	typedef void(*rtlsdr_read_async_cb_t)(unsigned char *buf, uint32_t len, void *ctx);
public:
	RTLDriver(QObject *parent);
	~RTLDriver();
	static quint32 getDeviceCount();
	static QString getDeviceName(const quint32 index);
	static bool getDeviceUSBStrings(const quint32 index,
		QString &manufact,
		QString &product,
		QString &serial);
	static bool open(const quint32 index);
	static bool close();
	static bool setXtalFreq(const quint32 rtlFreq, const quint32 tunerFreq);
	static bool getXtalFreq(quint32 &rtlFreq, quint32 &tunerFreq);
	//static bool writeEeprom(const quint8 const *data, const quint8 offset, const quint16 len);
	//static bool readEeprom(quint8 *data, const quint8 offset, const quint16 len);
	static bool setCenterFreq(const quint32 freq);
	static quint32 getCenterFreq();
	static bool setFreqCorrection(qint32 ppm);
	static qint32 getFreqCorrection();
	static TunerType getTunerType();
	static quint32 getTunerGains(QVector<qint32> &gains);
	static bool setTuerGain(const qint32 gain);	// 设置并返回当前准确的gain
	static qint32 getTunerGain();
	// 下面这两个函数还没有被提供
	//static quint32 getTunerBandWidth();
	//static bool setTunerBandWidth(const quint32 bw);	// 设置并返回当前准确的tuner带宽
	static bool setTunerGainMode(const GainMode mode);
	static bool setTestMode(const bool on);
	static bool setAgcMode(const bool on);
	static bool setSampleRate(quint32 rate);
	static quint32 getSampleRate();
	// 这里直接采样模式的设置，直接采样主要用于极低频率信号，暂时用不到，此函数这里引入只是为了确保关闭这个模式
	// 0 on disabled, 1 on I branch, 2 on Q branch
	static bool setSampleMode(const qint8 on);
	static qint8 getSampleMode();
	// static setDsMode(...);
	// 为零中频的tuner使用，如E4000系列
	static bool setOffsetTuning(const bool on);
	static qint8 getOffsetTuning();
	static bool resetBuffer();
	/*static bool readSync(QVector<qreal> *buf);*/
	//static bool waitAsync()
	static bool readAsync(rtlsdr_read_async_cb_t cb,
		void *ctx,
		quint32 bufNum = 0,
		quint32 bufLen = 0);
	static bool cancelAsync();
	

};

#endif // !RTLDRIVER_H
