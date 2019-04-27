#pragma once
#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
class Worker : public QThread
{
	Q_OBJECT

public:
	Worker(void (*task)(void));
	~Worker();
	void run()override;
private:
	void(*m_task)(void);	// the task of worker
};

#endif // !WORKER_H
