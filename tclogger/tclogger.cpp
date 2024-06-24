#include <iomanip>
#include <iostream>
#include <limits>
#include <ctime>
#include <chrono>
#include "tclogger.h"

#ifdef __linux__
#define localtime_s(tm, time) localtime_r(time, tm);
#endif

string 
TCLogger::getDate() const 
{
    time_t calTime = time(NULL);
    tm *loctime = localtime(&calTime);
    stringstream date;
    date << put_time(loctime, "%Y%m%d");

    return date.str();
}

string 
TCLogger::getTime() const 
{
    const auto timeNow = chrono::system_clock::now().time_since_epoch();
        
    time_t in_ms = chrono::duration_cast<chrono::milliseconds>(timeNow).count();
    time_t sec = in_ms / 1000;
    int ms = in_ms % 1000;

    tm localTime;
    localtime_s(&localTime, &sec);

    stringstream time;
    time << put_time(&localTime, "%H:%M:%S");
    time << "." << ms;

    return time.str();
}

TCLogger::TCLogger()
{
    string filename = "C:\\Projects\\Logger\\Data\\FinalBeforeSend" + getDate() + ".txt";
    elog_.open(filename, ios::app);
    if(!elog_){
        throw ios_base::failure("File Open Failure" + filename);
    }
}

TCLogger::~TCLogger()
{
    if(elog_.is_open()){
        elog_.close();
    }
}

void 
TCLogger::endTimedLog()
{
    elog_ << getTime() << " " << str() << endl;
    str("");
    clear();
}



