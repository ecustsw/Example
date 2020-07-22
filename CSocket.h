#pragma once
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
class CSocket
{
public:
	CSocket();
	~CSocket();
	int Create(int domain,int type);
	int Listen(const char *ip,short port);
    int Accept(CSocket &socket);
	int Connect(const char* ip, short port);
	int Close();
    int Attach(CSocket& other);
    bool Valid();
    const char* LastError();
    int isReadyRead(int seconds = 5);
    int isReadyWrite(int seconds = 5);
    
protected:
    enum SocketStatus
    {
        INVALID = -1
    };

private:
	int m_socket;
	int m_family;
};

