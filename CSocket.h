#pragma once
class CSocket
{
public:
	CSocket();
	~CSocket();
	int Create(int domain,int type);
	int Listen(const char *ip,short port);
    int Accept();
	int Connect(const char* ip, short port);
	int Close();

private:
    int isReady(int seconds = 5);
	int m_socket;
	int m_family;
};

