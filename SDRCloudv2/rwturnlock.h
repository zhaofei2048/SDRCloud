#pragma once
#ifndef RWTURNLOCK_H
#define RWTURNLOCK_H

#include <QObject>
QT_FORWARD_DECLARE_CLASS(QWaitCondition);
QT_FORWARD_DECLARE_CLASS(QMutex);
// RWTurnLock：读者-写者轮换锁
// Description：这个锁用于读者写者需要严格轮换操作一块共享内存的场景
// Public Method:
// lockForWrite()
// lockForRead
// unlock
class RWTurnLock
{
public:
	RWTurnLock();
	~RWTurnLock();
	void lockForWrite();
	void lockForRead();
	void unlock();
private:
	bool isFull;
	QMutex *bufMutex;
	QWaitCondition *bufNotFull;
	QWaitCondition *bufNotEmpty;
};

#endif // !RWTURNLOCK_H
