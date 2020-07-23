#pragma once
#include <ctime>
#include "CThreadBase.h"
#include "CMessage.h"

class CFtpWorker :
    public CThreadBase
{
public:
    CFtpWorker();
    void processMsg();

protected:
    virtual void run();
    virtual void onLogOn();
    virtual void onLogOut();
    virtual void onTestRequest();
    virtual void onHeatbeat();
    virtual void onBusiness();
    virtual void logOn();
    virtual void logOut();
    virtual void testRequest();
    virtual void heatbeat();
    virtual void business();

private:
    time_t m_lastActiveTime;
    time_t m_timeInterval;
    CMessage m_msg;
};

