#include <iostream>
#include "tclogger.h"
//#include "tctcpclient.h"

int main(){
    ELOG << "Hello World" << endtl;

    ELOG << "This is Alex" << endtl;

    ELOG << "Have a nice day" << endtl;

    cout << "Message Using Cout" << endtl;

    TCtcpClient tcpClient ("127.0.0.1", 12345);
    tcpClient.connect();

    if (tcpClient.connected()) 
    {
        tcpClient.processMsgs();
        tcpClient.disconnect();
    }

    /*
    int result = tcp_client();
    if(result != 0){
        cerr << "TCP client encountered an error :( " << endl;
    }
    */


}



