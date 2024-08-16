#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tcmanager.h"
#include "tclogger.h"
//#include "tctcpclient.h"
//#include "tccontrolif.h"

using namespace std;

TCManager::TCManager(const string& remoteHost, int remotePort) 
 : controlIF_()
 , tcpClient_(remoteHost, remotePort) {}

TCManager::~TCManager() 
{
    shutDown();    
}

bool
TCManager::init()
{

    if (!controlIF_.initTcpServer() || !tcpClient_.connect())
    {
        return false; 
    }
    cout << "TWSClient Is Ready" << endl;
    ELOG << "TWSClient Is Ready" << endtl;

    return true;
}

void
TCManager::processMsgs()
{
    while (true)
    {
        fd_set readFDs;
        FD_ZERO(&readFDs);
        FD_SET(tcpClient_.getFD(), &readFDs);

        int max_fd = tcpClient_.getFD();

        if (controlIF_.acceptingConnections()) 
        {
            FD_SET(controlIF_.server_fd(), &readFDs);
        }

        max_fd = max(max_fd, controlIF_.server_fd());

        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        ELOG << "PM: " <<"Calling Select" << endtl;

        ELOG << "PM: " << "Enter A Message: " << endtl;
        cout << "PM: " << "Enter A Message: " << endl;

        int rv = select(max_fd + 1, &readFDs, NULL, NULL, &tv); 

        if (rv == -1)
        {
            cerr << "PM: " << "Error With Select Function() " << strerror(errno) << endl;
            ELOG << "PM: " <<"Error With Select Function() " << strerror(errno) << endtl;
            tcpClient_.disconnect();
            return;
        }

        if (rv == 0)
        {
            cerr << "PM: " << "Data Unavailable... " << strerror(errno) << endl;
            ELOG << "PM: " << "Data Unavailable... " << strerror(errno) << endtl;
            continue;
        } 

        if (FD_ISSET(tcpClient_.getFD(), &readFDs))
        {   
            if (!tcpClient_.receive()) 
            {
                return;
            } 
            else 
            {
                cout << "PM: " << "Message Sucessfully Received!" << endl;
            }
        }

        if (FD_ISSET(controlIF_.server_fd(), &readFDs)) 
        {
           controlIF_.acceptConnection(); 
        }

    }
}

void
TCManager::shutDown()
{
    controlIF_.shutdownTcpServer();
    tcpClient_.disconnect();
    ELOG << "TCManager Shutting Down" << endtl; 
}
