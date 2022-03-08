#ifndef _socket_hpp_
#define _socket_hpp_
#include "ServerClass.hpp"

int acceptNewClient(const int& endpoint, sockaddr_in* addr = 0, socklen_t* len_s = 0);

char *getIPAddress(const int& socket);

#endif
