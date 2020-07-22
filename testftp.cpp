#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "CSocket.h"
#include "CFtpWorker.h"

int main(int argc,char *argv[])
{
    int ret = 0;
    CSocket clientSocket;
    const char *ip = "127.0.0.1";
    short port = 9100;
    clientSocket.Create(AF_INET,SOCK_STREAM);
    if(clientSocket.Connect(ip,port) != 0)
    {
        printf("client socket connect error:%s\n",clientSocket.LastError());
        return -1;
    }
    sleep(10);
    clientSocket.Close();

    return 0;
}