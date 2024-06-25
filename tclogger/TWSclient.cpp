#include <iostream>
#include "tclogger.h"


int main(){
    TCLogger& logger = TCLogger::getInstance();
    logger << "Hello World";
    logger.endTimedLog();

    logger << "This is Alex";
    logger.endTimedLog();

    logger << "Have a nice day";
    logger.endTimedLog(); 
}



