#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "CSocket.h"

CSocket::CSocket()
{
    m_socket = -1;
}

CSocket::~CSocket()
{

}

int CSocket::Create(int domain, int type)
{
    m_family = domain;
    m_socket = socket(domain,type,0);
    return m_socket;
}

int CSocket::Listen(const char* ip, short port)
{
    int ret = 0;
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    int len = sizeof(addr);
    if((ret = bind(m_socket,(struct sockaddr*)&addr,(socklen_t)len)) != 0)
        return ret;

    if((ret = listen(m_socket,0)) != 0)
        return ret;

    return ret;
}

int CSocket::Accept()
{
    int peer = 0;
    struct sockaddr_in peerAddr;
    memset(&peerAddr,0,sizeof(peerAddr));
    int len = sizeof(peerAddr);
    if((peer = accept(m_socket,(sockaddr*)&peerAddr,(socklen_t*)&len)) != 0)
        return peer;
    printf("peer port is %d,ip is %s\n",ntohs(peerAddr.sin_port),inet_ntoa(peerAddr.sin_addr));
    return peer;
}

int CSocket::Connect(const char* ip, short port)
{
    int ret = 0;
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    int len = sizeof(addr);
    if((ret = connect(m_socket,(struct sockaddr*)&addr,(socklen_t)len) != 0))
        return ret;

    return ret;
}

int CSocket::Close()
{
    if (m_socket >= 0)
    {
        close(m_socket);
    }
}

int CSocket::isReady(int seconds)
{
    fd_set rfds,wfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(m_socket,&rfds);
    FD_SET(m_socket,&wfds);

    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    retval = select(1, &rfds, &wfds, NULL, &tv);

    return retval;
}