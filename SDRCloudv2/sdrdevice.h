#pragma once
#ifndef SDRDEVICE_H
#define	SDRDEVICE_H

#include <QObject>
//QT_FORWARD_DECLARE_CLASS(QQueue<qreal>)
#include <QQueue>
class SDRDevice : public QObject
{
	Q_OBJECT
public:
	typedef enum DEV_STATE_FLAG{CLOSED, OPENED, INACTIVE, ACTIVE}DEV_STATE;
	typedef struct sdrdev dev_t;
public:
	SDRDevice(QQueue<qreal> *queueBufX, QQueue<qreal> *queueBufY, QObject *parent);
	~SDRDevice();
public:
	DEV_STATE getState();	// 获取设备当前状态
	virtual quint32 getDeviceCount() = 0;	// 获取可搜寻到的设备的数量
	virtual QString getDeviceName(quint32 index) = 0;	// 获取设备身份名称
	virtual bool getDeviceUSBInfo(quint32 index,
		QString &manufact, QString &product, QString &serial) = 0;	// 获取设备的USB信息
	virtual bool open(quint32 index) = 0;	// 根据索引打开一个SDR设备
	virtual bool close() = 0;	// 关闭一个SDR设备
	virtual bool readAsyncData(quint16 bufNum = 0, quint32 bufLen = 0) = 0;	// 异步读取数据

protected:
	dev_t *m_dev;	// SDR设备的句柄（供驱动内部传递信息用，我们不用了解）
	DEV_STATE m_state;
	QQueue<qreal> *m_QueueBufX;
	QQueue<qreal> *m_QueueBufY;
};
#endif // !SDRDEVICE_H
