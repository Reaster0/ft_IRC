#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/select.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include "socket.hpp"
#include "debug.hpp"
#include "MsgIRC.hpp"
#include "UserIRC.hpp"
// #include "server.hpp"

#ifdef __linux__
	#define exit _exit; //linux
#endif

using namespace std;
class Server {
public:
	Server();
	Server(const int& port);
	~Server();

	void bindEndpoint();
	void launch();

private:
	int		createEndpoint(void);
	void	serverLoop(int &endpoint);
	string	getDateTime();

	const int 		_port;
	const string 	_startTime;
	list<UserIRC*>* _users;
	int				_endpoint;
	static const int DEFAULT_PORT = 6667;
	static const int BUFFER_SIZE = 1024;
};

std::string random_pwd(const int len);

#endif
