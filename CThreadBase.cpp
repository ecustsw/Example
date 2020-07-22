#include <exception>
#include <cstdio>
#include <cstdlib>

#include "CThreadBase.h"

void* CThreadBase::ThreadProc(void* attr)
{
	printf("thread start\n");
	int *ret = (int*)malloc(sizeof(int));
	*ret = 0;
	CThreadBase* pThis = (CThreadBase*)attr;
	pThis->m_status = ThreadStatus::WORKING;
	try
	{
		pThis->run();
	}
	catch (...)
	{
		printf("thread run error\n");
		*ret = -1;
		pthread_exit(ret);
	}
	pThis->m_status = ThreadStatus::INVALID;
	printf("thread terminated\n");
	pthread_exit(ret);
}	

bool CThreadBase::start()
{
	int ret = false;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//no need join

	if (pthread_create(&m_threadId, &attr, ThreadProc, this) == 0)
		ret = true;

	pthread_attr_destroy(&attr);

	return ret;
}

int CThreadBase::cancel()
{
	int ret = 0;
	ret = pthread_cancel(m_threadId);
	return ret;
}

int CThreadBase::join()
{
	int ret = 0;
	void *pRetCode = NULL;
	ret = pthread_join(m_threadId, &pRetCode);
	m_ret = *((int*)pRetCode);
	free(pRetCode);
	return ret;
}