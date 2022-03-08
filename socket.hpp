#ifndef _socket_hpp_
#define _socket_hpp_
#include "ServerClass.hpp"

using namespace std;

struct UserIRC;

int acceptNewClient(const int& endpoint, sockaddr_in* addr = 0, socklen_t* len_s = 0);

#endif
