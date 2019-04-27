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
	if (isFull == true)
	{
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
