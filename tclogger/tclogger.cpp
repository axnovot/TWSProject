#include <iomanip>
#include <iostream>
#include <limits>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include "tclogger.h"

#ifdef __linux__
#define localtime_s(tm, time) localtime_r(time, tm);
#endif


TCLogger& TCLogger::getInstance()
{
    static TCLogger instance;
    return instance;
}

TCLogger::TCLogger()
{
    string filename = elogFilePath() + "twsclient_elog_" + getDate() + ".txt";
    elog_.open(filename, ios::app);
    if (!elog_) {
        throw ios_base::failure("File Open Failure" + filename);
    }
}

TCLogger::~TCLogger()
{
    if (elog_.is_open()) {
        elog_.close();
    }
}

string
TCLogger::elogFilePath() const
{
    const char* data_area = getenv("TC_DATA_AREA");
    if (data_area != nullptr) {
        cout << "The data area is set to: " << data_area << endl;
        return data_area;
    } else {
        cerr << "TC_DATA_AREA env is not set" << endl;
        throw ios_base::failure("TC_DATA_AREA env is not set");
    }
}

void 
TCLogger::endTimedLog()
{
    elog_ << getTime() << " " << str() << endl;
    str("");
    clear();
}

string 
TCLogger::getDate() const 
{
    time_t calTime = time(NULL);
    tm loctime; 
    localtime_s(&loctime,&calTime);
    stringstream date;
    date << put_time(&loctime, "%Y%m%d");

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

ostream& operator<<(ostream& logger, endtl_t) 
{
    TCLogger* pTCLogger = dynamic_cast<TCLogger*>(&logger);
    if (pTCLogger) {
        cout << "Cast Successful" << endl;
        pTCLogger->endTimedLog();
        return logger;
    } else {
        cout << " : This Message Is Not On Logger Instance" << endl;
        return logger << endl;
    }
}

void endtl()
{

}
