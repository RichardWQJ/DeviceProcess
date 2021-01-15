#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "../include/Util.h"
#include "Singleton.hpp"
#include "Thread.h"

class Handler : public Singleton<Handler>, public Thread
{
    friend class Singleton<Handler>;

public:
    Handler();
    ~Handler();

    void RemoveCurrProcessDevice();

    int HandlerTimerStart();
    int HandlerTimerStop();

    void HandlerMsg(std::string &strMsg);

private:
    virtual void run() override;
    void DeviceProcess();

    void ResetHandlerTimeDoneFlag(bool isTimeDone);

    int HandlerTimerInit();
    int HandlerTimerDelete();

    timer_t m_timerId;

    std::mutex m_mtxQueue;
    std::priority_queue<std::string> m_deviceQueue;

    std::string m_currDevice;
};

#endif //!_HANDLER_H_