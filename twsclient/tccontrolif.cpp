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
#include "tcmanager.h"

using namespace std;

extern bool KeepRunning;

TCControlIF::TCControlIF(const TCManager& tcManager) 
: tcManager_(tcManager) {}

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
        ELOG << "CI: " << "TCControlIF Accepting Connections On: " << ServerPort << endl;
        return true;
    }
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
        cout << "CI-IN: " << received_msg << endl;
        ELOG << "CI-IN: " << received_msg << endtl;

        vector<string> words = splitMsg(received_msg);
        const string command = words[0];
        words.erase(words.begin());
        const vector<string>& args = words;
    
        const string ciResponse = handleControlMsg(command, args);

        cout << "CI-OUT: " << ciResponse << endl;
        ELOG << "CI-OUT: " << ciResponse << endtl;
        send(new_socket, ciResponse.c_str(), ciResponse.size(), 0);
       
    }

    close(new_socket);

}

string
TCControlIF::handleControlMsg(const string& command, const vector<string>& args) const
{
    string response;
    
    if (command == "ping")
    {
        response = "ACK";
    }
    else if (command == "help")
    {
        response = "Supported Commands: ping, help, send, quit";
    }
    else if (command == "send" && !args.empty())
    {   
        string msg;
        for (string::size_type i = 0; i < args.size(); ++i) 
        {
            msg += args[i];
            if (i < args.size()-1) 
            {
                msg += " ";
            }
        }
        tcManager_.tcpClient().send(msg);
        response = "Sent: " + msg;
    }
    else if (command == "quit") 
    {
        KeepRunning = false;
        response = "Quit Command Called";
    }
    else 
    {
        response = command + ": Is Not Supported";
    }

    return response + "\n";
}

vector<string>
TCControlIF::splitMsg(const string& received_msg) const 
{
    vector<string> words;
    string word;

    for (string::size_type i = 0; i < received_msg.size(); ++i) 
    {
        if (!isspace(received_msg[i])) 
        {
            word += received_msg[i];
        }
        else 
        {
            if (!word.empty()) 
            {
                words.push_back(word);
                word.clear();
            }
        }
    }

    if (!word.empty()) 
    {
        words.push_back(word);
    }

    return words;

}

void
TCControlIF::shutdownTcpServer() 
{
    if (acceptingConnections())
    {
        close(server_fd_);
        server_fd_ = -1;
        
        cout << "CI: " << "TCControl Server Shutting Down, No Longer Listening On " << ServerPort << endl;
        ELOG << "CI: " << "TCControl Server Shutting Down, No Longer Listening On " << ServerPort << endtl;
    }
}
