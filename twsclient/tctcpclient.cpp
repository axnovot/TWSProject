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


TCTcpClient::~TCTcpClient() 
{
    disconnect();
}

bool
TCTcpClient::connect()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0)
    {
        cerr << "TC: " << "Socket Creation Failure" << strerror(errno) << endl;
        ELOG << "TC: " << "Socket Creation Failure" << strerror(errno) << endtl;
        fd_ = -1;
        return false;
    }
        
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(remotePort_);
    server_address.sin_addr.s_addr = inet_addr(remoteHost_.c_str());

    if (::connect(fd_, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) 
    {
        cerr << "TC: " << "Connection To Server Failed" << strerror(errno) << endl;
        ELOG << "TC: " << "Connection To Server Failed" << strerror(errno) << endtl;
        close(fd_);
        fd_ = -1;
        return false;
    } 
    else 
    {

        int flags = fcntl(fd_, F_GETFL, 0);
        if (fcntl(fd_, F_SETFL, flags | O_NONBLOCK) < 0) 
        {
            cerr << "TC: " << "Error Making Connection Nonblocking :(" << strerror(errno) << endl;
            ELOG << "TC: " << "Error Making Connection Nonblocking :(" << strerror(errno) << endtl;
        }

        cout << "TC: " << "Connected To : " << remoteHost_ << ":" << remotePort_ << endl;
        ELOG << "TC: " << "Connected To : " << remoteHost_ << "." << remotePort_ << endtl;
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
        cout << "TC: " << "TCTcpClient Disconnected" << endl;
        ELOG << "TC: " << "TCTcpClient Disconnected" << endtl;
    }        
}


bool
TCTcpClient::send(const string& msg) const
{
    int sent = ::send(getFD(),msg.c_str(), msg.size(),0);
    cout << "TC-OUT: " << msg << endl;
    ELOG << "TC-OUT: " << msg << endtl;
    if (sent < 0) 
    {
        cerr << "TC: " << "Send Failed" << strerror(errno) << endl;
        ELOG << "TC: " << "Send Failed" << strerror(errno) <<endtl;
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
    string message;
    while (true) 
    {
        const int bytes_received = recv(getFD(), buffer, maxRecvSize, 0);
        if (bytes_received < 0) 
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
                break;
            }
            else 
            {
                cerr << "TC: " << "Message Reception Failure" << strerror(errno) <<endl;
                ELOG << "TC: " << "Message Reception Failure" << strerror(errno) << endtl;
                return false;
            }
        }
        else if (bytes_received == 0) 
        {
            cerr << "TC: " << "Server Quit " << strerror(errno) << endl;
            ELOG << "TC: " <<"Server Quit " << strerror(errno) << endtl;
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
    cout << "TC-IN: " << message << endl;
    ELOG << "TC-IN: " << message << endtl;
    return true;
}
