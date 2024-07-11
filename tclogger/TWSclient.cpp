#include <iostream>
#include "tclogger.h"
#include "tctcpclient.h"
#include "tcconfig.h"

int main(){
    ELOG << "Hello World" << endtl;

    ELOG << "This is Alex" << endtl;

    ELOG << "Have a nice day" << endtl;

    cout << "Message Using Cout" << endtl;

    TCConfig& tcconfig = TCConfig::getInstance();

    TCTcpClient tcpClient (tcconfig.tcp_server_host(), tcconfig.tcp_server_port());
    tcpClient.connect();

    if (tcpClient.connected()) 
    {
        tcpClient.processMsgs();
        tcpClient.disconnect();
    }
}
