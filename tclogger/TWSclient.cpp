#include "tclogger.h"
#include <iostream>

int main(){
    TCLogger log;

    log << "Hello World"; 
    log.endTimedLog();
    
    log << "This is Alex"; 
    log.endTimedLog();

    log << "Have a nice day"; 
    log.endTimedLog(); 
}


