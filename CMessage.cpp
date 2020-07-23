#ifndef __MESSAGE_H_
#define __MESSAGE_H_
#include <cstring>
#include "CMessage.h"

void CMessage::resetMsg(int msgCode)
{
    m_msgCode = msgCode;
    m_pData = NULL;
    memset(m_msg,0x00,sizeof(m_msg));
    m_pData = m_msg + sizeof(int);
}

int CMessage::setMsg(int value)
{
    *((int*)m_pData) = value;
    m_pData = m_msg + sizeof(int);
}

int CMessage::setMsg(const char *value)
{
    int len = strlen(value);
    if(len > (m_msg + MAX_MSG_LEN - 1 - m_pData))
        len = m_msg + MAX_MSG_LEN - 1 - m_pData;

    memcpy(m_pData,value,len);
    m_pData += len;
}

int CMessage::parseMsg(const char *buf)
{
    m_msgCode = *(int*)buf;
    if(buf != m_msg)
        memcpy(m_msg,buf,sizeof(m_msg) - 1);
    m_pData = m_msg + sizeof(int);
}

char* CMessage::getMsg()
{
    return m_msg;
}

int CMessage::Size()
{
    return m_pData - m_msg;
}

#endif