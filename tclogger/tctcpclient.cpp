#include <iostream>
#include <cstring>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include "tctcpclient.h"
#include "tcconfig.h"
#include "tclogger.h"

using namespace std;

TCTcpClient::TCTcpClient(const string& remoteHost, int remotePort)
 :remoteHost_(remoteHost)
 ,remotePort_(remotePort)
 ,fd_(-1) 
{
}

bool
TCTcpClient::connect()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0)
    {
        cerr << "Socket Connection Creation Failure" << endl;
        fd_ = -1;
        return false;
    }
        
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(remotePort_);
    server_address.sin_addr.s_addr = inet_addr(remoteHost_.c_str());

    if (::connect(fd_, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        cerr << "Connection To Server Failed" << endl;
        close(fd_);
        fd_ = -1;
        return false;
    } else {
        cout << "Connected To : " << remoteHost_ << ":" << remotePort_ << endl;
        return true;
    }    
}

void
TCTcpClient::disconnect()
{
    if (connected())
    {
        close(fd_);
        fd_ = -1;
        cout << "Connection Closed" << endl;
    }        
}

void
TCTcpClient::processMsgs() 
{
    while (true) 
    {  
        fd_set readfiles;
        FD_ZERO(&readfiles);
        FD_SET(STDIN_FILENO, &readfiles);
        FD_SET(fd_, &readfiles);

        int max_fd = max(STDIN_FILENO, fd_);
        ELOG << "Max_fd: " << max_fd << endtl;

        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        ELOG << "Monitoring file descriptors: " << endtl;
        ELOG << "Socket fd_: " << fd_ << endtl;
        ELOG << "STDIN_FILENO: " << STDIN_FILENO << endtl; 
        
        ELOG << "Calling Select" << endtl;

        ELOG << "Enter A Message: " << endtl;
        cout << "Enter A Message: " << endl;

        int rv = select(max_fd + 1, &readfiles, NULL, NULL, &tv);
        ELOG << "select value: " << rv << endtl;

        if (rv == -1) 
        {
            cerr << "Error With Select Function()" << endl;
            disconnect();
            return;
        }
        
        if (rv == 0) 
        {
            cerr << "Data Unavailable..." << endl;
            continue;      
        }
       
        if (FD_ISSET(STDIN_FILENO, &readfiles))
        {
            ELOG << "FD_ISSET for STDIN_FILENO" << endtl;
            string input;
            getline(cin, input);
            ELOG << "Message Entered: " <<input << endtl;

            if (cin.eof()) 
            {
                cerr << "Quit Command Recieved, Exiting..." << endl;
                disconnect();
                break;
            }

            if (send(fd_, input.c_str(), input.size(), 0) < 0) 
            {
                cerr << "Send Failed" << endl;
                disconnect();
                return;
            }
        }

        if (FD_ISSET(fd_, &readfiles)) 
        { 
            ELOG << "FD_ISSET for fd_ socket" << endtl;
            char buffer[1024];
            int bytes_received = recv(fd_, buffer, sizeof(buffer)-1, 0);
            if (bytes_received < 0) 
            {
                cerr << "Message Reception Failure" << endl;
                disconnect();
                return;
            }

            else if (bytes_received == 0) 
            {
                cerr << "Server Quit" << endl;
                disconnect();
                return;
            }
            buffer[bytes_received] = '\0';
            cout << "Server Response: " << buffer << endl;
            ELOG << "Server Response: " << buffer << endtl;
        }
    }     
}


