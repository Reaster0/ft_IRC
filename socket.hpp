#ifndef _socket_hpp_
#define _socket_hpp_
#include "server.hpp"

int acceptNewClient(const int& endpoint);

char *getIPAddress(const int& socket);

#endif