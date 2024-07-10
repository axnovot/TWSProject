#ifndef tcconfig_h_INCLUDED
#define tcconfig_h_INCLUDED

#include <string>

using namespace std;

class TCConfig
{
public:
    static TCConfig& getInstance();
    string tcp_server_host() const;
    unsigned int tcp_server_port() const;

private:
    TCConfig();
    ~TCConfig();
    string tcp_server_host_;
    unsigned int tcp_server_port_;
};
#endif
