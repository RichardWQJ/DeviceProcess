#include "Handler.h"
#include "Listener.h"

std::mutex g_mtxTimeDone;
static bool g_isHandlerTimeDone = true;

Handler::Handler()
{
    DebugPrint("Handler Constructed\n");
    HandlerTimerInit();
}

Handler::~Handler()
{
    DebugPrint("Handler Destructed\n");
    HandlerTimerDelete();
}

static void TimerThreadFun(union sigval arg)
{
    ErrPrint("Handler: A round of timing has been completed.\n");
    std::lock_guard<std::mutex> lockGuard(g_mtxTimeDone);
    g_isHandlerTimeDone = true;
}

int Handler::HandlerTimerInit()
{
    struct sigevent event;
    struct sched_param param;
    int policy, retCode;

    retCode = pthread_getschedparam(pthread_self(), &policy, &param);
    if (retCode != EOK)
    {
        ErrPrint("Failed to Get Sched Param. ErrCode:[%s].\n", strerror(retCode));
        exit(1);
    }

    param.sched_priority = 15;
    policy = SCHED_RR;
    retCode = pthread_setschedparam(pthread_self(), policy, &param);
    if (retCode != EOK)
    {
        ErrPrint("Failed to Set Sched Param. ErrCode:[%s].\n", strerror(retCode));
        exit(1);
    }

    event.sigev_notify = SIGEV_THREAD;
    event.sigev_value.sival_ptr = NULL;
    event.sigev_notify_function = TimerThreadFun;
    event.sigev_notify_attributes = NULL;

    retCode = timer_create(CLOCK_MONOTONIC, &event, &this->m_timerId);
    if (retCode != EOK)
    {
        ErrPrint("Failed to Create Timer. ErrCode:[%s].\n", strerror(retCode));
        exit(1);
    }

    return retCode;
}

int Handler::HandlerTimerStart()
{
    struct itimerspec iTimer;
    int retCode;

    /* The time wait for first start. */
    iTimer.it_value.tv_sec = 1;
    iTimer.it_value.tv_nsec = 500000;

    /* the interval after start. */
    iTimer.it_interval.tv_sec = 0;
    iTimer.it_interval.tv_nsec = 0;

    retCode = timer_settime(this->m_timerId, 0, &iTimer, NULL);
    if (retCode != EOK)
    {
        ErrPrint("Failed to Set Time. ErrCode:[%s].\n", strerror(retCode));
    }

    return retCode;
}

int Handler::HandlerTimerStop()
{
    struct itimerspec iTimer;
    int retCode;

    /* The time wait for first start.When value is 0, mean stop it. */
    iTimer.it_value.tv_sec = 0;
    iTimer.it_value.tv_nsec = 0;

    /* the interval after start. */
    iTimer.it_interval.tv_sec = 0;
    iTimer.it_interval.tv_nsec = 0;

    retCode = timer_settime(this->m_timerId, 0, &iTimer, NULL);
    if (retCode != EOK)
    {
        ErrPrint("Failed to Set Timer. ErrCode:[%s].\n", strerror(retCode));
    }

    return retCode;
}

int Handler::HandlerTimerDelete()
{
    int retCode = timer_delete(this->m_timerId);
    if (retCode != EOK)
    {
        ErrPrint("Failed to Delete Timer. ErrCode:[%s].\n", strerror(retCode));
    }

    return retCode;
}

void Handler::HandlerMsg(std::string &strMsg)
{
    std::lock_guard<std::mutex> mtxGuard(m_mtxQueue);
    if (!strMsg.empty() && strMsg != m_currDevice)
    {
        this->m_deviceQueue.push(strMsg);
    }
}

void Handler::run()
{
    while (1)
    {
        if (!g_isHandlerTimeDone)
        {
            InfoPrint("Timing Has NOT been Completed!\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            DeviceProcess();
        }
    }
}

void Handler::DeviceProcess()
{
    InfoPrint("Processing Device......\n");
    std::lock_guard<std::mutex> mtxGuard(m_mtxQueue);
    if (!this->m_deviceQueue.empty())
    {
        
        ErrPrint("Queue Size:%d.\n", this->m_deviceQueue.size());
        string strCurrDevice = this->m_deviceQueue.top();
        if (m_currDevice == strCurrDevice)
        {
            WarnPrint("Process Device:[%s] Still......\n", strCurrDevice.c_str());
            return;
        }
        m_currDevice = strCurrDevice;
        InfoPrint("========>>>> Current Process Device:[%s]\n", m_currDevice.c_str());
        ResetHandlerTimeDoneFlag(false);
        HandlerTimerStart();
        Listener::GetInstance()->ListenerTimerStart(4);
    }
    else
    {
        ErrPrint("There is NO device waiting for Process!\n");
        m_currDevice.clear();
    }
}

void Handler::RemoveCurrProcessDevice()
{
    WarnPrint("RemoveCurrProcessDevice......\n");
    std::lock_guard<std::mutex> lockGuard(m_mtxQueue);
    this->m_deviceQueue.pop();
}

void Handler::ResetHandlerTimeDoneFlag(bool isTimeDone)
{
    DebugPrint("Reset Handler Timing Flag.\n");
    std::lock_guard<std::mutex> lockGuard(g_mtxTimeDone);
    g_isHandlerTimeDone = isTimeDone;
}