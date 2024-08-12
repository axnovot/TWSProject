#ifndef tccontrolif_h_INCLUDED
#define tccontrolif_h_INCLUDED

using namespace std;

class TCControlIF {
public:
    TCControlIF();
    ~TCControlIF();

    bool initTcpServer();

    int server_fd() const;
    
    bool acceptingConnections() const;
    void acceptConnections();

    void shutdownTcpServer();

private:
    constexpr static int ServerPort{32201};
    int server_fd_{-1};
};

#endif

