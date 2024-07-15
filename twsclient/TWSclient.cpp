#include <iostream>
#include "tclogger.h"
#include "tctcpclient.h"
#include "tcconfig.h"
#include "tcmanager.h"

int main(){
    ELOG << "TWSClient Starting" << endtl;

    TCConfig& tcconfig = TCConfig::getInstance();

    TCManager theManager(tcconfig.tcp_server_host(), tcconfig.tcp_server_port());

    if (theManager.init()) 
    {
        theManager.processMsgs();
    }
    theManager.shutDown();
}
