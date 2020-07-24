#include <cstdio>
#include <exception>
#include <ctime>
#include "CFtpWorker.h"

CFtpWorker::CFtpWorker()
{
    m_timeInterval = 30;
}

void CFtpWorker::processMsg()
{
    switch(m_msg.m_msgCode)
    {
        case CMessage::LOGON :
            onLogOn();
            break;
        case CMessage::LOGOUT :
            onLogOut();
            break;
        case CMessage::TESTREQUEST :
            onTestRequest();
            break;
        case CMessage::HEARTBEAT :
            onHeatbeat();
            break;
        case CMessage::BUSINESS :
            onBusiness();
            break;
        default:
            printf("unsupported msg code\n");
            return;
    }
    m_lastActiveTime = time(NULL);
}

void CFtpWorker::run()
{
    m_status = ThreadStatus::WORKING;
    m_lastActiveTime = time(NULL);
    while(m_socket.Valid())
    {
        time_t now = time(NULL);
        int ret = m_socket.isReadyRead();
        if (ret > 0)
        {
            if(m_socket.Recv(MAX_MSG_LEN - 1,m_msg.getMsg()) < 0)
                break;
            m_msg.parseMsg(m_msg.getMsg());
            processMsg();
        }
        else
        {
            testRequest();
            if (m_lastActiveTime + m_timeInterval < now)
            {
                m_socket.Close();
                break;
            }
        }        
    }	
	return;
}

void CFtpWorker::onLogOn()
{
    m_msg.resetMsg(CMessage::LOGON);
    m_msg.setMsg("onLogon");
    m_socket.Send(m_msg.Size(),m_msg.getMsg());
}

void CFtpWorker::onLogOut()
{
    m_msg.resetMsg(CMessage::LOGOUT);
    m_msg.setMsg("onLogout");
    m_socket.Send(m_msg.Size(),m_msg.getMsg());
    m_socket.Close();
}

void CFtpWorker::onTestRequest()
{
    m_msg.resetMsg(CMessage::HEARTBEAT);
    m_msg.setMsg("Heartbeat");
    m_socket.Send(m_msg.Size(),m_msg.getMsg());    
}

void CFtpWorker::onHeatbeat()
{

}

void CFtpWorker::onBusiness()
{

}

void CFtpWorker::logOn()
{
    m_msg.resetMsg(CMessage::LOGON);
    m_msg.setMsg("Logon");
    m_socket.Send(m_msg.Size(),m_msg.getMsg());
}

void CFtpWorker::logOut()
{
    m_msg.resetMsg(CMessage::LOGOUT);
    m_msg.setMsg("Logout");
    m_socket.Send(m_msg.Size(),m_msg.getMsg());
    m_socket.Close();
}

void CFtpWorker::testRequest()
{
    m_msg.resetMsg(CMessage::TESTREQUEST);
    m_msg.setMsg("TestRequest");
    m_socket.Send(m_msg.Size(),m_msg.getMsg());  
}

void CFtpWorker::heatbeat()
{
    m_msg.resetMsg(CMessage::HEARTBEAT);
    m_msg.setMsg("Heartbeat");
    m_socket.Send(m_msg.Size(),m_msg.getMsg());  
}

void CFtpWorker::business()
{

}
