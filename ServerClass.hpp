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

#include <map>
#include "UserIRC.hpp"
#include "socket.hpp"
#include "debug.hpp"
#include "MsgIRC.hpp"
#include "Channel.hpp"
// #include "server.hpp"

#ifdef __linux__
	#define exit _exit; //linux
#endif

class UserList;
class Channel;

using namespace std;
class Server {
public:
	Server();
	Server(const int& port);
	~Server();
	
	void launch();

private:
	void 	bindEndpoint();
	int		createEndpoint(void);
	void	serverLoop(int &endpoint);
	string	getDateTime();

	const int 				_port;
	const string 			_startTime;
	UserList*				_users;
	map<string, Channel>	_channels;
	int						_endpoint;

	static const int DEFAULT_PORT = 6667;
	static const int BUFFER_SIZE = 1024;
};

std::string random_pwd(const int len);

#endif
