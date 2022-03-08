#ifndef _my_debug_
#define _my_debug_
#include "ServerClass.hpp"

using namespace std;

struct PayloadIRC;

void sendToAll(fd_set& availableSockets, const int& maxSockets , const char* msg, const int& exception);

int testMessagesForAll(const int& i, fd_set& availableSockets, const int& maxSockets);

int printConnexion(const int& i);

void printPayload(PayloadIRC& payload);

#endif
