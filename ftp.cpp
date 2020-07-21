#include <cstdio>
#include <cstdlib>
#include "CFtpWorker.h"

int main(int argc,char *argv[])
{
	CFtpWorker worker;
	worker.start();
	if (worker.join() == 0)
	{
		printf("thread ret code is %d\n",worker.m_ret);
	}
	return 0;
}