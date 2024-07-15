#ifndef TCManager_h_INCLUDED
#define TCManager_h_INCLUDED

#include <string>
#include "tctcpclient.h"
#include "tclogger.h"

using namespace std;

class TCManager{
public:
    TCManager(const string& remoteHost, int remotePort);
   
    bool init();
    void processMsgs();
    void shutDown();
private:
    
    TCTcpClient tcpClient_;
};

#endif
