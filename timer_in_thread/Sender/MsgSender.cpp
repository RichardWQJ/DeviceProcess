#include "MsgSender.h"

MsgSender::MsgSender()
{
    SharedMemInit();
}

MsgSender::~MsgSender()
{
}

void MsgSender::SharedMemInit()
{
    pthread_mutexattr_t mtxShareMemDataAttr;
    this->m_fdShareMem = shm_open(SHARE_MEM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);

    if (this->m_fdShareMem < 0)
    {
        if (this->m_fdShareMem = shm_open(SHARE_MEM_NAME, O_RDWR | O_CREAT, S_IRWXU) < 0)
        {
            ErrPrint("Failed to open shared data [%s]!\n", SHARE_MEM_NAME);
            exit(1);
        }
    }

    ftruncate(this->m_fdShareMem, sizeof(SharedMem));
    this->m_ptrShareMem = (SharedMem *)mmap(0, sizeof(SharedMem), PROT_READ | PROT_WRITE,
                                            MAP_SHARED, m_fdShareMem, 0);

    pthread_mutexattr_init(&mtxShareMemDataAttr);
    pthread_mutexattr_setpshared(&mtxShareMemDataAttr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&m_ptrShareMem->mtxSharedMemData, &mtxShareMemDataAttr);
}

void MsgSender::SendData(std::string &strData)
{
    pthread_mutex_lock(&this->m_ptrShareMem->mtxSharedMemData);
    memset(this->m_ptrShareMem->data, 0, sizeof(this->m_ptrShareMem->data));
    strcpy(this->m_ptrShareMem->data, strData.c_str());
    pthread_mutex_unlock(&this->m_ptrShareMem->mtxSharedMemData);
}