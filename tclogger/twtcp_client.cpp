#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int 
tcp_client()
{
    //create socket(this is a connectoin endpoint that allows for programs to
    //send and recieve data over a network) enables a client and network to 
    //establish a connection and exchange information

    int sockConnection = socket(AF_INET ,SOCK_STREAM ,0); //adress family, socket type, protocol
    if (sockConnection < 0)
    {
        cerr << "Socket Connection Creation Failure" << endl;
        return 1;
    }

    //holds servers adress and port information    

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; //sets adress family for socket
    server_address.sin_port = htons(12345); //port number, ensures number is correctly interpreted by network
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); //sets ip adress of server

    if (connect(sockConnection, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        cerr << "Connection Failed" << endl;
        close(sockConnection);
        return 1;
    }

    while (true)
    {
        string input;
        cout << "Enter A Message: ";
        getline(cin, input);

        if (send(sockConnection, input.c_str(), input.size(), 0) < 0){
            cerr << "Send Failed" << endl;
            close(sockConnection);
            return 1;
        }

        char buffer[1024];
        int bytes_received = recv(sockConnection, buffer, sizeof(buffer)-1, 0);
        if (bytes_received < 0)
        {
            cerr << "Message Reception Failure" << endl;
            close(sockConnection);
            return 1;

        }

        buffer[bytes_received] = '\0';
        cout << "Server Response: " << buffer << endl;
    }    
     
    close(sockConnection);
    return 0;
}
