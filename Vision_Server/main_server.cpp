#include <cstdlib>
#include <iostream>

#include "Thread.h"
#include "NetThread.h"
#include "gsNetPoll_g++.h"



int main()
{
    NetThread *net = new NetThread();

    net->SetOutputStream(stdout);
    net->OpenReadPort(2070);
    net->start();


    while(1)
    {
        DoSleep(1000);
    }

    return 0;
}

