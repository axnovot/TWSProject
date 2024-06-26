#ifndef tclogger_h_INCLUDED
#define tclogger_h_INCLUDED

#include <sstream>
#include <fstream>

using namespace std;

class TCLogger : public ostringstream {
public:
    static TCLogger& getInstance();
    void endTimedLog();

private:
    TCLogger();
    ~TCLogger();

    string elogFilePath() const;
    string getDate() const;
    string getTime() const;
   
private:
    ofstream elog_;

};
typedef void endtl_t();
void endtl();

ostream& operator<<(ostream& logger, endtl_t);

#endif






