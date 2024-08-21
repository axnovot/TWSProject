#include <iostream>
#include <csignal>
#include "tclogger.h"
#include "tctcpclient.h"
#include "tcconfig.h"
#include "tcmanager.h"
//#include "tccontrolif.h"

volatile bool KeepRunning = false;

void sigint_handler(int) 
{
    KeepRunning = false;
}


int main() {
    ELOG << "TWSClient Starting" << endtl;

    TCConfig& tcconfig = TCConfig::getInstance();

    TCManager theManager(tcconfig.tcp_server_host(), tcconfig.tcp_server_port());
    
    signal(SIGINT, sigint_handler);

    if (theManager.init()) 
    {
        KeepRunning = true;
        while (KeepRunning == true)
        {        
            theManager.processMsgs();
        }
    }
    theManager.shutDown();
}
