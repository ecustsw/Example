#include <cstdio>
#include <exception>
#include <ctime>
#include "CFtpWorker.h"

void CFtpWorker::run()
{
    m_status = ThreadStatus::WORKING;
    while(m_socket.Valid())
    {
        printf("do work\n");
        sleep(10);
    }	

	return;
}