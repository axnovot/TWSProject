#ifndef TCTcpClient_h_INCLUDED
#define TCTcpClient_h_INCLUDED

#include <string>

using namespace std;

class TCTcpClient{
public:
    TCTcpClient(const string& remoteHost, int remotePort);

    bool connect();
    
    inline bool connected() const {return fd_ >= 0;}
    void disconnect();

    void processMsgs();

private:
    string remoteHost_;
    int remotePort_;
    int fd_;

};
#endif
