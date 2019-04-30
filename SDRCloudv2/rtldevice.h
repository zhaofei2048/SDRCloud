#pragma once
/*
	rtl设备类
*/
#include <QObject>
#include <QVector>
#include "readwritebuffer.h"
#include <QThread>

#define SAMPLE_RATE_COUNT 11
QT_FORWARD_DECLARE_CLASS(ReadDataWorker)

class RtlDevice : public QObject
{
	Q_OBJECT

public:
	RtlDevice(QObject *parent);
	~RtlDevice();
	friend void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx);
	static const QString sampleModeStrings[3];
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
	typedef enum SampleMode_flag
	{
		Quadrature = 0,
		DI,
		DQ

	}SampleMode;
	typedef enum State_flag
	{
		CLOSED = 0,
		OPENED,
		RUNNING,
		CANCELLING,
		LOST
	}State;
	
private:
	static const quint32 DEFAULT_SAMPLERATES[11];	// Hz
public:	// 暴露出的接口函数
	static QVector<quint32> getDefaultSampleRateList();
	bool getDeviceList(QVector<QString> &names, quint32 &count);
	QString getDeviceName();
	bool open(const quint32 index);	// 打开rtl设备，并获取rtl设备的基本信息
	bool close();	// 关闭已经打开的rtl设备
	bool startReadData();
	bool stopReadData();
	State getState();
	SampleMode getSampleMode();
	quint32 getSampleRateIndex();
	bool getOffsetTuningState();
	bool getTunerAgcState();
	bool getRtlAgcState();
	void getTunerGainRange(quint32 &min, quint32 &max);
	quint32 getTunerGain();
	void getData(QVector<qreal>& data);


public:
	bool m_readData();	// 这个函数应该由readDataSignal进行调用
signals:
	void readDataSignal(RtlDevice *dev);	// 此方法会阻塞，应该放到worker里执行
public slots:
	void readDataStopSlot();
private:
	void initRtl();
	bool stopRunning();	// 停止数据的读取
private:
	// 设备的状态参数
	//bool m_isOpen;	// 设备是否打开了
	//bool m_isRunning;	// 设备正在运行读取数据
	State m_state;	// 设备的状态
	bool m_isBufferReset;	// 缓冲区是否复位
	QString m_name;	// 设备的名称

	// tuner相关
	TunerType m_tunerType;	// tuner类型
	quint32 m_tunerFreq;	// tuner的频率
	QVector<qint32> m_tunerGainList;	// 设备支持的增益列表
	qint32 m_tunerGain;	// tuner的增益
	GainMode m_tunerGainMode;	// tuner的增益模式
	bool m_isOffsetTuningOn;

	// RTL
	quint32 m_sampleRate;	// 采样率
	qint32 m_freqCorrection;	// 时钟频率误差校正
	bool m_isRtlAgcOn;	// 是否开启了RTL的AGC自动增益控制
	SampleMode m_sampleMode;

	// 其他
	quint32 m_downSampleRate1;	// 对数据进行的下采样率
	quint32 m_rtlXtalFreq;
	quint32 m_tunerXtalFreq;

	// 缓冲区
	ReadWriteBuffer<qreal> m_data;
	QThread readDataThread;
	ReadDataWorker *worker;
};
