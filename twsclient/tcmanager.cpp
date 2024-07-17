#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include "tcmanager.h"
#include "tclogger.h"
#include "tctcpclient.h"

using namespace std;

TCManager::TCManager(const string& remoteHost, int remotePort) 
 : tcpClient_(remoteHost, remotePort) {}


bool
TCManager::init()
{
    if (tcpClient_.connect()) 
    {
        cout << "Connection Successful" << endl;
        ELOG << "TWSClient Is Ready" << endtl;
        return true;
    } else {
        cout << "Connection Failed" << endl;
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

        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        ELOG << "PM: " <<"Calling Select" << endtl;

        ELOG << "PM: " <<"Enter A Message: " << endtl;
        cout << "Enter A Message: " << endl;

        int rv = select(max_fd + 1, &readFDs, NULL, NULL, &tv); 

        if (rv == -1)
        {
            cerr << "Error With Select Function()" << endl;
            ELOG << "PM: " <<"Error With Select Function()" << endtl;
            tcpClient_.disconnect();
            return;
        }

        if (rv == 0)
        {
            cerr << "Data Unavailable..." << endl;
            ELOG << "PM: " << "Data Unavailable..." << endtl;
            continue;
        }

        if (FD_ISSET(STDIN_FILENO, &readFDs))
        { 
            string input;
            getline(cin, input);
            ELOG << "PM: " << "Message Entered: " << input << endtl;

            if (cin.eof())
            {
                cerr << "Quit Command Recieved, Exiting..." << endl;
                ELOG << "PM: " << "Quit Command Received, Exiting..." << endtl;
                tcpClient_.disconnect();
                break;
            }

            if (!tcpClient_.send(input))
            {
                return;
            } else {
                cout << "Message Successfully Sent!" << endl;
            }
        }

        if (FD_ISSET(tcpClient_.getFD(), &readFDs))
        {   
            if (!tcpClient_.receive()) 
            {
                return;
            } else {
                cout << "Message Sucessfully Received!" << endl;
            }
        }
    }
}

void
TCManager::shutDown()
{
    tcpClient_.disconnect();
}

