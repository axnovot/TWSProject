#ifndef tccontrolif_h_INCLUDED
#define tccontrolif_h_INCLUDED

#include <vector>
#include <string>

class TCManager;

using namespace std;

//extern bool keepRunning;

class TCControlIF {
public:
    TCControlIF(const TCManager& tcManager);
    ~TCControlIF();

    bool initTcpServer();

    int server_fd() const { return server_fd_; }
    
    bool acceptingConnections() const { return server_fd() >= 0; }
    void acceptConnection();

    void shutdownTcpServer();

private:
    string handleControlMsg(const string& command, const vector<string>& args) const;
    vector<string> splitMsg(const string& received_msg) const;
private:
    const TCManager& tcManager_;
    
    const int ServerPort{32001};
    int server_fd_{-1};

};

#endif

