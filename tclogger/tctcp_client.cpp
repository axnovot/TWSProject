#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "tclogger.h"

using namespace std;

TCtcpClient::TCtcpClient(const string& remoteHost, int remotePort) : remoteHost_(remoteHost), remotePort_(remotePort), fd_(-1) {}

bool
TCtcpClient::connect()
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

bool
TCtcpClient::connected() const
{
    return fd_ >= 0;
}

void
TCtcpClient::disconnect()
{
    if (connected())
    {
        close(fd_);
        fd_ = -1;
        cout << "Connection Closed" << endl;
    }        
}

void
TCtcpClient::processMsgs() 
{
    while (true) 
    {
        string input;
        cout << "Enter A Message: " << endl;
        getline(cin, input);

        if (cin.eof())
        {
            cerr << "Quit Command Received, Exiting..." << endl;
            break;
        }

        if (send(fd_, input.c_str(), input.size(), 0) < 0) 
        {
            cerr << "Send Failed" << endl;
            disconnect();
            return;
        }
        
        char buffer[1024];
        int bytes_received = recv(fd_, buffer, sizeof(buffer)-1, 0);
        if (bytes_received < 0) 
        {
            cerr << "Message Reception Failure" << endl;
            disconnect();
            return;
        }

        buffer[bytes_received] = '\0';
        cout << "Server Response: " << buffer << endl;
    }
}
















