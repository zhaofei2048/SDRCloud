#include "rwturnlock.h"
#include <QMutex>
#include <QWaitCondition>

RWTurnLock::RWTurnLock()
{
	isFull = false;
	bufMutex = new QMutex;
	bufNotFull = new QWaitCondition;
	bufNotEmpty = new QWaitCondition;
}

RWTurnLock::~RWTurnLock()
{
	delete bufMutex;
	bufMutex = NULL;
	delete bufNotFull;
	bufNotFull = NULL;
	delete bufNotEmpty;
	bufNotEmpty = NULL;
}

void RWTurnLock::lockForWrite()
{
	bufMutex->lock();
	if (isFull == true)	// 注意正规的写法应该使用while对条件进行判断，但这里我确保线程被唤醒时它就该执行一次
	{					// 同时也是为了防止其中一个线程退出时，而另一个线程被永久阻塞下去
		bufNotFull->wait(bufMutex);
	}
	bufMutex->unlock();
}

void RWTurnLock::lockForRead()
{
	bufMutex->lock();
	if (isFull == false)
	{
		bufNotEmpty->wait(bufMutex);
	}
	bufMutex->unlock();
}

void RWTurnLock::unlock()
{
	bufMutex->lock();
	if (isFull == false)
	{
		// 写者轮换到读者
		isFull = true;
		bufNotEmpty->wakeAll();
	}
	else
	{
		// 读者轮换到写者
		isFull = false;
		bufNotFull->wakeAll();
	}
	bufMutex->unlock();
}