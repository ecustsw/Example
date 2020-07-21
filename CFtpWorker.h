#pragma once
#include "CThreadBase.h"
class CFtpWorker :
    public CThreadBase
{
protected:
    virtual void run();
};

