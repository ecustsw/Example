#pragma once

#define MAX_MSG_LEN 256
class CMessage
{
public:
    enum MsgCode
    {
        LOGON = 0,
        LOGOUT,
        TESTREQUEST,
        HEARTBEAT,
        BUSINESS
    };
    void resetMsg(int msgCode);
    int setMsg(int value);
    int setMsg(const char *value);
    virtual int parseMsg(const char *buf);
    virtual char* getMsg();

    int Size();
    int m_msgCode;
protected:
    char *m_pData;
    char m_msg[MAX_MSG_LEN];
};