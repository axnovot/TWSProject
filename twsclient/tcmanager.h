#ifndef TCManager_h_INCLUDED
#define TCManager_h_INCLUDED

#include <string>
#include "tctcpclient.h"
#include "tccontrolif.h"

using namespace std;

class TCManager {
public:
    TCManager(const string& remoteHost, int remotePort);
    ~TCManager();   

    bool init();
    void processMsgs();
    void shutDown();

    const TCTcpClient& tcpClient() const {return tcpClient_;}

private:
    TCControlIF controlIF_;
    TCTcpClient tcpClient_;
};

#endif
