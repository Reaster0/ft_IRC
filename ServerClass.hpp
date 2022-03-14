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
// #include "replies.hpp"
// #include "server.hpp"

#ifdef __linux__
	#define exit _exit; //linux
#endif

struct MsgIRC;
class Channel;

using namespace std;

# define SERVER_NAME "EpikEkipEkolegram"
class Server {
public:
	Server();
	Server(const int& port);
	~Server();
	
	void launch();
	const string& name() const;
	void sendMessage(UserIRC* receiver, PayloadIRC payload);
	void sendMessage(UserIRC* sender, UserIRC* receiver, PayloadIRC payload);

	UserList				_users;
	queue<MsgIRC>			_msgQueue;
	map<string, Channel>	_channels;
	const string 			_startTime;
	const string			_hostName;
	const string			_password;

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
void sendToAllChanInfo(PayloadIRC& payload, UserIRC *user, Server &server);

//remove the user from all channels it was connected
void removeUsersFromAllChans(UserIRC *user, Server &server);

bool chanExist(const string& channel, Server &server);

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
int MODEParser(MsgIRC& msg, Server& server);
int PRIVMSGParser(MsgIRC& msg, Server& server);
int WHOParser(MsgIRC& msg, Server& server);
int NAMESParser(MsgIRC& msg, Server& server);
int MOTD(MsgIRC& msg, Server& server);
int INFOParser(MsgIRC& msg, Server& server);
int AWAY(MsgIRC& msg, Server& server);
int TIME(MsgIRC& msg, Server& server);
int USERHOSTParser(MsgIRC& msg, Server& server);
int IsonParser(MsgIRC& msg, Server& server);
int PINGParser(MsgIRC& msg, Server& server);
int PONGParser(MsgIRC& msg, Server& server);
int PARTParser(MsgIRC& msg, Server& server);
int TOPICParser(MsgIRC& msg, Server& server);
int LISTParser(MsgIRC& msg, Server& server);
int KICKParser(MsgIRC& msg, Server& server);
int KILLParser(MsgIRC& msg, Server& server);
int INVITEParser(MsgIRC& msg, Server& server);
int WHOISParser(MsgIRC& msg, Server& server);

#endif
