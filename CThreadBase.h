#ifndef __THREADBASE_H_
#define __THREADBASE_H_

#include <pthread.h>
#include "CSocket.h"

class CThreadBase
{
public:
	bool start();
	int cancel();
	int join();

	static void* ThreadProc(void* attr);

	int m_ret;
    CSocket m_socket;

protected:

    enum ThreadStatus
    {
        INVALID = -1,
        WORKING,
        FREE
    };

	virtual void run() = 0;
	pthread_t m_threadId;
    ThreadStatus m_status;
};

#endif
