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
#include <fcntl.h>
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
    } 
    else 
    {

        int flags = fcntl(fd_, F_GETFL, 0);
        if (fcntl(fd_, F_SETFL, flags | O_NONBLOCK) < 0) 
        {
            cerr << "Error Making Connection Nonblocking :(" << endl;
        }

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


bool
TCTcpClient::send(const string& msg) 
{
    int sent = ::send(getFD(),msg.c_str(), msg.size(),0);
    cout << "TCP OUT: " << msg << endl;
    ELOG << "PM: " << "TCP OUT: " << msg << endtl;
    if (sent < 0) 
    {
        cerr << "Message Failed" << endl;
        ELOG << "Send Failed" << endtl;
        return false;
    } 
    else 
    {
        return true;
    }
}

bool
TCTcpClient::receive() 
{ 
    const int maxRecvSize = 10;
    char buffer[maxRecvSize + 1];
    int bytes_received;
    string message;
    while (true) 
    {
        bytes_received = recv(getFD(), buffer, maxRecvSize, 0);
        if (bytes_received < 0) 
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
                break;
            }
            else 
            {
                cerr << "Message Reception Failure" << endl;
                ELOG << "PM: " << "Message Reception Failure" << endtl;
                return false;
            }
        }
        else if (bytes_received == 0) 
        {
            cerr << "Server Quit" << endl;
            ELOG << "PM: " <<"Server Quit" << endtl;
            return false;
        } 
        else 
        {
            buffer[bytes_received] = '\0';
            message += buffer;
            
            if (bytes_received < maxRecvSize) 
            {
                break;
            }
        }
    }
    cout << "TCP IN: " << message << endl;
    ELOG << "PM: " << "TCP IN: " << message << endtl;
    return true;
}







