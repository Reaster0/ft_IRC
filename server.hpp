#ifndef SERVER
#define SERVER

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/select.h>
#include <netdb.h>
#include "clientList.hpp"


int create_endpoint(void);

//input the port number of the server and return the fd of the server socket
int create_server(const int &port_num);

//main loop for all of the execution
void server_loop(int &endpoint);

#endif