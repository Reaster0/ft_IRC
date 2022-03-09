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
#include <list>
#include <queue>
#include <sstream>
#include <cstring>
#include "MsgIRC.hpp"
#include "UserIRC.hpp"
#include "socket.hpp"
#include "debug.hpp"
#include "Channel.hpp"
// #include "server.hpp"

#ifdef __linux__
	#define exit _exit; //linux
#endif

class MsgIRC;
class Channel;

using namespace std;
class Server {
public:
	Server();
	Server(const int& port);
	~Server();
	
	void launch();
	UserList				_users;
	queue<MsgIRC>			_msgQueue;
	map<string, Channel>	_channels;
	const string 			_startTime;
	const string			_hostName;

private:
	void 	bindEndpoint();
	int		createEndpoint(void);
	void	serverLoop(int &endpoint);
	void	initializeMap();

	const int 				_port;
	int						_endpoint;
	map<string, int(*)(MsgIRC&, Server&)>	_handlerFunction;

	static const int DEFAULT_PORT = 6667;
	static const int BUFFER_SIZE = 1024;
};

std::string randomPwd(const int len);
string getDateTime();

//send a message to all users that are on the same channel that input user
void sendToAllChan(PayloadIRC& payload, UserIRC *user, Server &server);

//remove the user from all channels it was connected
void removeUsersFromAllChans(UserIRC *user, Server &server);

/*
-------handlerFunction--------
please put your functions here using the template [int function(MsgIRC&, ServerClass&)]
and in initializeMap() for the referencement
all functions here may broke msg so don't use it afterward
*/
int funCap(MsgIRC& msg, Server& server);
int NICKParser(MsgIRC& msg, Server& server);
int USERParser(MsgIRC& msg, Server& server);
int QUITParser(MsgIRC& msg, Server& server);
int JOINParser(MsgIRC& msg, Server& server);

#endif
