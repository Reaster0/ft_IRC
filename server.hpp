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

int create_endpoint(void);

//input the port number of the server and return the fd of the server socket
int create_server(const int &port_num);

//main loop for all of the execution
void server_loop(int &endpoint);

std::string random_pwd(const int len);

#endif