#ifndef _MSG_SENDER_H_
#define _MSG_SENDER_H_

#include "../include/Util.h"
#include "Singleton.hpp"

class MsgSender : public Singleton<MsgSender>
{
    friend class Singleton<MsgSender>;

public:
    MsgSender();
    ~MsgSender();

    void SharedMemInit();

    void SendData(std::string &strData);

private:
    int m_fdShareMem;
    SharedMem *m_ptrShareMem = NULL;
};

#endif //!_MSG_SENDER_H_