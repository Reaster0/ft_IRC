#ifndef _SERVER_
#define _SERVER_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/select.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <queue>
#include "socket.hpp"
#include "debug.hpp"
#include "MsgIRC.hpp"
#include "UserIRC.hpp"

#ifdef __linux__
			#define exit _exit; //linux
#endif

int createEndpoint(void);

//input the port number of the server and return the fd of the server socket
int createServer(const int &port_num);

//main loop for all of the execution
void serverLoop(int &endpoint);

std::string randomPwd(const int len);

#endif