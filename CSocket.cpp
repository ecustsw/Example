#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include "CSocket.h"

CSocket::CSocket()
{
    m_socket = SocketStatus::INVALID;
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
    int ov = 1;
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = m_family;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    int len = sizeof(addr);
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &ov, sizeof(ov));
    if((ret = bind(m_socket,(struct sockaddr*)&addr,(socklen_t)len)) != 0)
        return ret;

    if((ret = listen(m_socket,5)) != 0)
        return ret;

    return ret;
}

int CSocket::Accept(CSocket &socket)
{
    int peer = 0;
    struct sockaddr_in peerAddr;
    memset(&peerAddr,0,sizeof(peerAddr));
    int len = sizeof(peerAddr);
    if((peer = accept(m_socket,(sockaddr*)&peerAddr,(socklen_t*)&len)) < 0)
        return peer;
    socket.m_socket = peer;
    socket.m_family = m_family;
    printf("peer port is %d,ip is %s\n",ntohs(peerAddr.sin_port),inet_ntoa(peerAddr.sin_addr));
    return peer;
}

int CSocket::Connect(const char* ip, short port)
{
    int ret = 0;
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = m_family;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    int len = sizeof(addr);
    if((ret = connect(m_socket,(struct sockaddr*)&addr,(socklen_t)len) != 0))
        return ret;

    return ret;
}

int CSocket::Close()
{
    if (m_socket > SocketStatus::INVALID)
    {
        close(m_socket);
        m_socket = SocketStatus::INVALID;
    }
}

int CSocket::Attach(CSocket& other)
{
    m_socket = other.m_socket;
    m_family = other.m_family;
}

bool CSocket::Valid()
{
    return m_socket != SocketStatus::INVALID;
}

const char* CSocket::LastError()
{
    return strerror(errno);
}

int CSocket::isReadyRead(int seconds)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(m_socket,&rfds);

    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    retval = select(m_socket + 1, &rfds, NULL, NULL, &tv);

    return retval;
}

int CSocket::isReadyWrite(int seconds)
{
    fd_set wfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&wfds);
    FD_SET(m_socket,&wfds);

    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    retval = select(m_socket + 1, NULL, &wfds, NULL, &tv);

    return retval;
}