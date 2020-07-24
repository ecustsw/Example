#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "CSocket.h"
#include "CFtpWorker.h"
#include "CIni.h"

int main(int argc,char *argv[])
{
    CFtpWorker worker;
    int ret = 0;
    CSocket listenSocket;
    CIni ini;
    char ip[32] = {0};
    char tmp[32] = {0};
    short port = -1;
    ini.loadIni("config.ini");
    listenSocket.Create(AF_INET,SOCK_STREAM);
    ini.Get("COMMON","ip",ip);
    ini.Get("COMMON","port",tmp);
    port = atoi(tmp);
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