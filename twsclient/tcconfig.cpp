#include "tcconfig.h"

TCConfig::TCConfig()
 :tcp_server_host_("127.0.0.1")
 ,tcp_server_port_(12345)
{
}

TCConfig::~TCConfig() {}

TCConfig& TCConfig::getInstance() 
{
    static TCConfig instance;
    return instance;
}
