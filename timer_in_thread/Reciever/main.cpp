#include "Util.h"
#include "Listener.h"

int main()
{
    Listener *Listener = Listener::GetInstance();
    Listener->start();

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}