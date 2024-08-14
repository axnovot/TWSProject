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

private:
    TCControlIF controlIF_;
    TCTcpClient tcpClient_;
};

#endif
