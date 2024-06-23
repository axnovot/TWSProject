#ifndef tclogger_h_INCLUDED
#define tclogger_h_INCLUDED

#include <sstream>
#include <fstream>

using namespace std;

class TCLogger : public ostringstream {
public:
    TCLogger();
    ~TCLogger();

    void endTimedLog();
private:
    string getDate() const;
    string getTime() const;
private:
    ofstream elog_;

};
#endif






