#include "MsgSender.h"

int main()
{
    std::vector<std::string> strDevList;
    strDevList.emplace_back("Switch");
    strDevList.emplace_back("Plug");
    strDevList.emplace_back("SmartLock");
    strDevList.emplace_back("SmartSensor");
    strDevList.emplace_back("SmartRelay");
    strDevList.emplace_back("Phone");

    while (1)
    {
        for (auto iter : strDevList)
        {
            InfoPrint("Device Name:[%s]\n", iter.c_str());
            MsgSender::GetInstance()->SendData(iter);
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    return 0;
}