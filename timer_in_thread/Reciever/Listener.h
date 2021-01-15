#ifndef _LISTENER_H_
#define _LISTENER_H_

#include "../include/Util.h"
#include "Singleton.hpp"
#include "Handler.h"
#include "Thread.h"

class Listener : public Singleton<Listener>, public Thread
{
    friend class Singleton<Listener>;

public:
    Listener();
    ~Listener();

    void SharedMemInit();

    int ListenerTimerStart(int iTimeWaitBeforeStart);
    int ListenerTimerStop();

private:
    virtual void run() override;

    int ListenerTimerInit();
    int ListenerTimerDelete();

    void DataRecieve();

    timer_t m_timerId;
    Handler* m_pHandler;

    int m_fdShareMem;
    SharedMem *m_ptrShareMem = NULL;

    std::string m_previousDeviceName; 
};

#endif //!_LISTENER_H_