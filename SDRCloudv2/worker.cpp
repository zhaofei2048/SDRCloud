#include "worker.h"

Worker::Worker(void(*task)(void)):
	m_task(task)
{
}

Worker::~Worker()
{
}

void Worker::run()
{
	m_task();
}
