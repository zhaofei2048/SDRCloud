#pragma once
#ifndef RTLDEVICE_H
#define RTLDEVICE_H

#include "sdrdevice.h"
#include <QReadWriteLock>

#define DEFAULT_SAMPLE_RATE		24000
#define DEFAULT_BUF_LENGTH		(1 * 16384)
#define MAXIMUM_OVERSAMPLE		16
#define MAXIMUM_BUF_LENGTH		(MAXIMUM_OVERSAMPLE * DEFAULT_BUF_LENGTH)
#define AUTO_GAIN				-100
#define DEFAULT_BUFFER_DUMP		4096

#define FREQUENCIES_LIMIT		1024

QT_FORWARD_DECLARE_CLASS(QTimer)
class RTLDevice : public SDRDevice
{
	Q_OBJECT

public:
	RTLDevice(QQueue<qreal> *queueBufX, QQueue<qreal> *queueBufY, QObject *parent);
	~RTLDevice();
private:
	static QReadWriteLock buf16RWLock;
	static quint32 buf16Len;
	static qint16  buf16[MAXIMUM_BUF_LENGTH];	// 此块缓冲区需要配合buf16RWLock读写锁使用以防冲突
	friend static void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx);


private slots:
	void updateBuf();
public:
	quint32 getDeviceCount()override;	// 获取可搜寻到的设备的数量
	QString getDeviceName(quint32 index)override;	// 获取设备身份名称
	bool getDeviceUSBInfo(quint32 index,
		QString &manufact, QString &product, QString &serial)override;	// 获取设备的USB信息
	bool open(quint32 index)override;	// 根据索引打开一个SDR设备
	bool close()override;	// 关闭一个SDR设备
	bool readAsyncData(quint16 bufNum = 0, quint32 bufLen = 0)override;

private:
	QTimer *updBufTimer;

};

#endif // !RTLDEVICE_H