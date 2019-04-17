#pragma once
#ifndef READRTLTHREAD_H
#define READRTLTHREAD_H



#include <QObject>
#include <QThread>

QT_FORWARD_DECLARE_CLASS(RTLDevice)

class ReadRTLThread : public QThread
{
	Q_OBJECT

	void run()override;
public:
	ReadRTLThread(RTLDevice *dongle);
	~ReadRTLThread();
private:
	RTLDevice *m_dongle;
};
#endif // !READRTLTHRED_H