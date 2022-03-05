#ifndef _my_debug_
#define _my_debug_
#include "server.hpp"

using namespace std;

void sendToAll(fd_set& availableSockets, const int& maxSockets , const char* msg, const int& exception);

int testMessagesForAll(const int& i, fd_set& availableSockets, const int& maxSockets);

int printConnexion(const int& i);

#endif