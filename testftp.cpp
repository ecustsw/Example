#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "CSocket.h"
#include "CFtpWorker.h"
#include "CIni.h"

int main(int argc,char *argv[])
{
    int ret = 0;
    CSocket clientSocket;
    CIni ini;
    char ip[32] = {0};
    char tmp[32] = {0};
    short port = -1;
    ini.loadIni("config.ini");
    clientSocket.Create(AF_INET,SOCK_STREAM);
    ini.Get("COMMON","ip",ip);
    ini.Get("COMMON","port",tmp);
    port = atoi(tmp);
    if(clientSocket.Connect(ip,port) != 0)
    {
        printf("client socket connect error:%s\n",clientSocket.LastError());
        return -1;
    }
    sleep(10);
    clientSocket.Close();

    return 0;
}