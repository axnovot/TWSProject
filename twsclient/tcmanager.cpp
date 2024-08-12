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
#include "tctcpclient.h"
#include "tccontrolif.h"

using namespace std;

TCControlIF controlIF_;

TCManager::TCManager(const string& remoteHost, int remotePort) 
 : tcpClient_(remoteHost, remotePort) {}

TCManager::~TCManager() 
{
    shutDown();    
}

bool
TCManager::init()
{

    if(!controlIF_.initTcpServer())
    {
        cerr << "Server Bind Failed, Unable To Listen To Incoming Connections " <<strerror(errno) << endl;
        return false;
    }
    else
    {
        cout << "Server Binded and Listening On Port" << endl;
        ELOG << "Server Binded and Listening On Port" << endtl;
    }

    if (tcpClient_.connect()) 
    {
        cout << "Connection Successful" << endl;
        ELOG << "TWSClient Is Ready" << endtl;
        return true;
    } 
    else 
    {
        cerr << "Connection Failed" << endl;
        return false;
    }
}

void
TCManager::processMsgs()
{
    while (true)
    {
        fd_set readFDs;
        FD_ZERO(&readFDs);
        FD_SET(STDIN_FILENO, &readFDs);
        FD_SET(tcpClient_.getFD(), &readFDs);

        int max_fd = max(STDIN_FILENO, tcpClient_.getFD());

        if (controlIF_.acceptingConnections()) 
        {
            FD_SET(controlIF_.server_fd(), &readFDs);
        }

        max_fd = max(max_fd, controlIF_.server_fd());

        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        ELOG << "PM: " <<"Calling Select" << endtl;

        ELOG << "PM: " <<"Enter A Message: " << endtl;
        cout << "Enter A Message: " << endl;

        int rv = select(max_fd + 1, &readFDs, NULL, NULL, &tv); 

        if (rv == -1)
        {
            cerr << "Error With Select Function()" << strerror(errno) << endl;
            ELOG << "PM: " <<"Error With Select Function()" << strerror(errno) << endtl;
            tcpClient_.disconnect();
            return;
        }

        if (rv == 0)
        {
            cerr << "Data Unavailable..." << strerror(errno) << endl;
            ELOG << "PM: " << "Data Unavailable..." << strerror(errno) << endtl;
            continue;
        }

        if (FD_ISSET(STDIN_FILENO, &readFDs))
        { 
            string input;
            getline(cin, input);
            ELOG << "PM: " << "Message Entered: " << input << endtl;

            if (cin.eof())
            {
                cerr << "Quit Command Recieved, Exiting..." << strerror(errno) << endl;
                ELOG << "PM: " << "Quit Command Received, Exiting..." << strerror(errno) << endtl;
                tcpClient_.disconnect();
                break;
            }

            if (!tcpClient_.send(input))
            {
                return;
            } 
            else 
            {
                cout << "Message Successfully Sent!" << endl;
            }
        }

        if (FD_ISSET(tcpClient_.getFD(), &readFDs))
        {   
            if (!tcpClient_.receive()) 
            {
                return;
            } 
            else 
            {
                cout << "Message Sucessfully Received!" << endl;
            }
        }

        if (FD_ISSET(controlIF_.server_fd(), &readFDs)) 
        {
           controlIF_.acceptConnections(); 
        }

    }
}

void
TCManager::shutDown()
{
    controlIF_.shutdownTcpServer();
    ELOG << "Server Shutting Down, No Longer Listening" << endtl;
    cout << "Server Shutting Down, No Longer Listening" << endl;
    tcpClient_.disconnect();
    ELOG << "TCManager Shutting Down" << endtl; 
}
