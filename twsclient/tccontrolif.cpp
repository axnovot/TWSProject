#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <algorithm>
#include <string>
#include <cctype>
#include "tccontrolif.h"
#include "tclogger.h"

using namespace std;

TCControlIF::~TCControlIF()
{
    shutdownTcpServer();
}

bool
TCControlIF::initTcpServer() 
{
    if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        cerr << "CI: " << "Socket Creation Failed " << "Error: " << strerror(errno) << endl;
        ELOG << "CI: " << "Socket Creation Failed " << "Error: " << strerror(errno) << endtl;
        return false;
    }
    
    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        cerr << "CI: " << "Setsockopt Failed" << "Error: " << strerror(errno) << endl;
        ELOG << "CI: " << "Setsockopt Failed" << "Error: " << strerror(errno) << endtl;
        return false;
    }

    struct sockaddr_in control_address;
    control_address.sin_family = AF_INET;
    control_address.sin_addr.s_addr = INADDR_ANY;
    control_address.sin_port = htons(ServerPort);

    if (bind(server_fd_, (struct sockaddr*)&control_address, sizeof(control_address)) < 0)
    {
        cerr << "CI: " << "Socket Bind Failed " << "Error: " << strerror(errno) << endl;
        ELOG << "CI: " << "Socket Bind Failed " << "Error: " << strerror(errno) << endtl;
        return false;
    }

    if (listen(server_fd_, 3) < 0)
    {
        cerr << "CI: " << "Listen Failed " << "Error: " << strerror(errno) << endl;
        ELOG << "CI: " << "Listen Failed " << "Error: " << strerror(errno) << endtl;
        return false;
    }
    else
    {
        cout << "CI: " << "TCControlIF Accepting Connections On: " << ServerPort << endl;
        ELOG << "CI: " << "TCCOntrolIF Accepting Connections On: " << ServerPort << endl;
        return true;
    }
}

int
TCControlIF::server_fd() const
{
    return server_fd_;
}

bool
TCControlIF::acceptingConnections() const
{
    return server_fd() >= 0;
}

void
TCControlIF::acceptConnection()
{
    if (!acceptingConnections())
    {
        return;
    }

    int new_socket;
    struct sockaddr client_address;
    int addrlen = sizeof(client_address);

    if ((new_socket = accept(server_fd_, (struct sockaddr *)&client_address, (socklen_t *)&addrlen)) < 0)
    {
        cerr << "CI: " << "Accept Failed..." << "Error: " << strerror(errno) << endl;
        ELOG << "CI: " << "Accept Failed..." << "Error: " << strerror(errno) << endtl;
        return;
    }

    char buffer[1024] = {0};
    int bytes_from_newsock = read(new_socket, buffer, sizeof(buffer));
    if (bytes_from_newsock > 0) 
    {
        string received_msg(buffer, bytes_from_newsock);
        
        received_msg.erase(remove_if(received_msg.begin(), received_msg.end(), ::isspace), received_msg.end());
        string response;

        cout << "CI-IN: " << "<" << received_msg << ">" << endl;
        ELOG << "CI-IN: " << received_msg << endtl;

        if (received_msg == "ping")
        {
            response = "ACK \n";
        }
        else if (received_msg == "help")
        {
            response = "Supported Commands: ping, help \n";
        }
        else
        {
            response = received_msg + ": Is Not Supported \n";
        }
        
        cout << "CI-OUT: " << response << endl;
        ELOG << "CI-OUT: " << response << endtl;
        send(new_socket, response.c_str(), response.size(), 0);
    }

    close (new_socket);
}

void
TCControlIF::shutdownTcpServer() 
{
    if (acceptingConnections())
    {
        close(server_fd_);
        server_fd_ = -1;
        
        cout << "CI-OUT: " << "TCControl Server Shutting Down, No Longer Listening" << endl;
        ELOG << "CI-OUT: " << "TCControl Server Shutting Down, No Longer Listening" << endtl;
    }
}






