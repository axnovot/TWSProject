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
        fd_set readfiles;
        FD_ZERO(&readfiles);
        FD_SET(STDIN_FILENO, &readfiles);
        FD_SET(tcpClient_.fd_, &readfiles);

        int max_fd = max(STDIN_FILENO, tcpClient_.fd_);
        ELOG << "PM: " << "Max_fd: " << max_fd << endtl;

        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        ELOG << "PM: " << "Monitoring file descriptors: " << endtl;
        ELOG << "PM: " <<"Socket fd_: " << tcpClient_.fd_ << endtl;
        ELOG << "PM: " <<"STDIN_FILENO: " << STDIN_FILENO << endtl;

        ELOG << "PM: " <<"Calling Select" << endtl;

        ELOG << "PM: " <<"Enter A Message: " << endtl;
        cout << "Enter A Message: " << endl;

        int rv = select(max_fd + 1, &readfiles, NULL, NULL, &tv);
        ELOG << "PM: " <<"select value: " << rv << endtl;

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

        if (FD_ISSET(STDIN_FILENO, &readfiles))
        {
            ELOG << "PM: " <<"FD_ISSET for STDIN_FILENO" << endtl;
            string input;
            getline(cin, input);
            ELOG << "PM: " << "Message Entered: " <<input << endtl;

            if (cin.eof())
            {
                cerr << "Quit Command Recieved, Exiting..." << endl;
                ELOG << "PM: " << "Quit Command Received, Exiting..." << endtl;
                tcpClient_.disconnect();
                break;
            }

            if (send(tcpClient_.fd_, input.c_str(), input.size(), 0) < 0)
            {
                cerr << "Send Failed" << endl;
                ELOG << "PM: " << "Send Failed" << endtl;
                tcpClient_.disconnect();
                return;
            }
        }

        if (FD_ISSET(tcpClient_.fd_, &readfiles))
        {
            ELOG << "PM: " <<"FD_ISSET for fd_ socket" << endtl;
            char buffer[1024];
            int bytes_received = recv(tcpClient_.fd_, buffer, sizeof(buffer)-1, 0);
            if (bytes_received < 0)
            {
                cerr << "Message Reception Failure" << endl;
                ELOG << "Message Reception Failure" << endtl;
                tcpClient_.disconnect();
                return;
            }

            else if (bytes_received == 0)
            {
                cerr << "Server Quit" << endl;
                ELOG << "PM: " <<"Server Quit" << endtl;
                tcpClient_.disconnect();
                return;
            }
            buffer[bytes_received] = '\0';
            cout << "Server Response: " << buffer << endl;
            ELOG << "PM: " <<"Server Response: " << buffer << endtl;
        }
    }
}

void
TCManager::shutDown()
{
    tcpClient_.disconnect();
}

