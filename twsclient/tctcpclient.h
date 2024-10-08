#ifndef TCTcpClient_h_INCLUDED
#define TCTcpClient_h_INCLUDED

#include <string>

using namespace std;

class TCTcpClient {
public:
    TCTcpClient(const string& remoteHost, int remotePort);
    ~TCTcpClient();

    bool connect();
    
    bool connected() const { return fd_ >= 0; }
    void disconnect();
   
    int getFD() const { return fd_; }

    bool send(const string& msg) const;
    bool receive() const;
    
private:
    string remoteHost_;
    int remotePort_;
    int fd_;
};
#endif
