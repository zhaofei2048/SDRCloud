#pragma once
#ifndef READDATAWORKER_H
#define READDATAWORKER_H


#include <QObject>
QT_FORWARD_DECLARE_CLASS(RtlDevice)

class ReadDataWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(RtlDevice *dev);
signals:
	void done();
};
#endif // !READDATAWORKER_H
