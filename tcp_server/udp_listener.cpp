// UDP Listener as written by copilot

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Set up the server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    serverAddr.sin_port = htons(12345); // Choose your desired port number

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Listening on UDP port 12345..." << std::endl;

    // Receive data
    char buffer[1024];
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    ssize_t receivedBytes;

    while (true) {
        receivedBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientLen);
        if (receivedBytes == -1) {
            std::cerr << "Error receiving data." << std::endl;
            continue;
        }

        // Process the received data (you can replace this with your own logic)
        std::cout << "Received: " << buffer << std::endl;
    }

    // Close the socket
    close(sockfd);

    return 0;
}
