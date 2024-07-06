#include <iostream>
#include "tclogger.h"
#include "tctcpclient.h"

int main(){
    ELOG << "Hello World" << endtl;

    ELOG << "This is Alex" << endtl;

    ELOG << "Have a nice day" << endtl;

    cout << "Message Using Cout" << endtl;

    int result = tcp_client();
    if(result != 0){
        cerr << "TCP client encountered an error :( " << endl;
    }
}



