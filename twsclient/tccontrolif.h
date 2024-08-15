#ifndef tccontrolif_h_INCLUDED
#define tccontrolif_h_INCLUDED

using namespace std;

class TCControlIF {
public:
    ~TCControlIF();

    bool initTcpServer();

    int server_fd() const {return server_fd_;}
    
    bool acceptingConnections() const {return server_fd() >= 0;}
    void acceptConnection();

    void shutdownTcpServer();

private:
    const int ServerPort{32001};
    int server_fd_{-1};
};

#endif

