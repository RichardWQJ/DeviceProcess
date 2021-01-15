#include "Listener.h"

sem_t g_Sem;

Listener::Listener() : m_pHandler(Handler::GetInstance())
{
    DebugPrint("Listener Constructed\n");
    SharedMemInit();
    ListenerTimerInit();
    sem_init(&g_Sem, 0, 0);
}

Listener::~Listener()
{
    DebugPrint("Listener Destructed\n");
    if (this->m_ptrShareMem != NULL)
    {
        munmap(m_ptrShareMem, sizeof(SharedMem));
        close(m_fdShareMem);
        shm_unlink(SHARE_MEM_NAME);
        ListenerTimerDelete();
    }
    ListenerTimerDelete();
}

void Listener::SharedMemInit()
{
    while ((this->m_fdShareMem = shm_open(SHARE_MEM_NAME, O_RDWR, S_IRWXU)) == -1)
    {
        ErrPrint("Failed to open shared data [%s]!\n", SHARE_MEM_NAME);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    this->m_ptrShareMem = (SharedMem *)mmap(0, sizeof(SharedMem), PROT_READ | PROT_WRITE,
                                            MAP_SHARED, this->m_fdShareMem, 0);
}

static void TimerThreadFun(union sigval arg)
{
    ErrPrint("Listener: A round of timing has been completed.\n");
    sem_post(&g_Sem);
}

int Listener::ListenerTimerInit()
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

int Listener::ListenerTimerStart(int iTimeWaitBeforeStart)
{
    struct itimerspec iTimer;
    int retCode;

    /* The time wait for first start. */
    iTimer.it_value.tv_sec = iTimeWaitBeforeStart;
    iTimer.it_value.tv_nsec = 0;

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

int Listener::ListenerTimerStop()
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

int Listener::ListenerTimerDelete()
{
    int retCode = timer_delete(this->m_timerId);
    if (retCode != EOK)
    {
        ErrPrint("Failed to Delete Timer. ErrCode:[%s].\n", strerror(retCode));
    }

    return retCode;
}

void Listener::run()
{
    m_pHandler->start();

    std::thread msgRecvTh(std::mem_fn(&Listener::DataRecieve), this);
    msgRecvTh.detach();

    while (1)
    {
        CriticalPrint("RUN......\n");
        sem_wait(&g_Sem);
        this->m_previousDeviceName.clear();
        this->m_pHandler->RemoveCurrProcessDevice();
    }
}

void Listener::DataRecieve()
{
    while (1)
    {
        pthread_mutex_lock(&this->m_ptrShareMem->mtxSharedMemData);

        std::string strDataRecieve = this->m_ptrShareMem->data;
        if (!strDataRecieve.empty() && this->m_previousDeviceName != strDataRecieve)
        {
            this->m_pHandler->HandlerMsg(strDataRecieve);
            this->m_previousDeviceName = strDataRecieve;
            memset(this->m_ptrShareMem->data, 0, sizeof(this->m_ptrShareMem->data));
        }

        pthread_mutex_unlock(&this->m_ptrShareMem->mtxSharedMemData);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}