#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "CSocket.h"
#include "CFtpWorker.h"

int main(int argc,char *argv[])
{
    CFtpWorker worker;
    int ret = 0;
    CSocket listenSocket;
    const char *ip = "127.0.0.1";
    short port = 9100;
    listenSocket.Create(AF_INET,SOCK_STREAM);
    if(listenSocket.Listen(ip,port) != 0)
    {
        printf("server socket listen error:%s\n",listenSocket.LastError());
        return -1;
    }
    while(1)
    {
        CSocket tmpSocket;
        if((ret = listenSocket.isReadyRead()) < 0)//wait for link
        {
            printf("server socket error\n");
            return -1;
        }
        else if(ret == 0)
        {
            printf("no link\n");
            continue;
        }
        if((ret = listenSocket.Accept(tmpSocket)) < 0)
        {
            printf("server socket accept error\n");
            return -1;
        }
        printf("have a link\n");        
        worker.m_socket.Attach(tmpSocket);
        worker.start();
    }
	return 0;
}