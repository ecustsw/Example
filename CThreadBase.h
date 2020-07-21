#ifndef __THREADBASE_H_
#define __THREADBASE_H_

#include <pthread.h>

class CThreadBase
{
public:
	bool start();
	int cancel();
	int join();

	static void* ThreadProc(void* attr);

	int m_ret;

protected:
	virtual void run() = 0;

private:
	pthread_t m_threadId;
};

#endif
