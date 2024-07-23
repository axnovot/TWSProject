#ifndef TCManager_h_INCLUDED
#define TCManager_h_INCLUDED

#include <string>
#include "tctcpclient.h"

using namespace std;

class TCManager {
public:
    TCManager(const string& remoteHost, int remotePort);
    ~TCManager();   

    bool init();
    void processMsgs();
    void shutDown();
private:
    
    TCTcpClient tcpClient_;
};

#endif
