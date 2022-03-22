#include "ServerClass.hpp"

using namespace std;

bool g_exit = false;

void Server::initializeMap()
{
	_handlerFunction["CAP"]			= funCap;
	_handlerFunction["NICK"]		= NICKParser;
	_handlerFunction["USER"]		= USERParser;
	_handlerFunction["QUIT"]		= QUITParser;
	_handlerFunction["JOIN"]		= JOINParser;
	_handlerFunction["MODE"]		= MODEParser;
	_handlerFunction["PRIVMSG"]		= PRIVMSGParser;
	_handlerFunction["WHO"]			= WHOParser;
	_handlerFunction["NAMES"]		= NAMESParser;
	_handlerFunction["MOTD"] 		= MOTD;
	_handlerFunction["INFO"]		= INFOParser;
	_handlerFunction["TIME"] 		= TIME;
	_handlerFunction["USERHOST"] 	= USERHOSTParser;
	_handlerFunction["AWAY"] 		= AWAY;
	_handlerFunction["ison"] 		= IsonParser;
	_handlerFunction["ISON"] 		= IsonParser;
	_handlerFunction["PING"] 		= PINGParser;
	_handlerFunction["PART"]		= PARTParser;
	_handlerFunction["TOPIC"] 		= TOPICParser;
	_handlerFunction["LIST"] 		= LISTParser;
	_handlerFunction["KICK"]		= KICKParser;
	_handlerFunction["KILL"]		= KILLParser;
	_handlerFunction["INVITE"]		= INVITEParser;
	_handlerFunction["WHOIS"]		= WHOISParser;
	_handlerFunction["WHOWAS"]		= WHOWASParser;
	_handlerFunction["ADMIN"]		= ADMINParser;
	_handlerFunction["LUSERS"]		= LUSERSParser;
	_handlerFunction["OPER"]		= OPERATORParser;
	_handlerFunction["PASS"]		= PASSParser;
	_handlerFunction["NOTICE"]		= PRIVMSGParser;
}

Server::Server() : _startTime(getDateTime()), _hostName(SERVER_NAME), _password("password"), _port(DEFAULT_PORT), _endpoint(createEndpoint())
{
	initializeMap();
	bindEndpoint();
	std::cout << "password:\n	" << _password << std::endl;
}

Server::Server(const int& port, const string& pwd) : _startTime(getDateTime()), _hostName(SERVER_NAME), _password(pwd), _port(port), _endpoint(createEndpoint())
{
	initializeMap();
	bindEndpoint();
	std::cout << "password:\n	" << _password << std::endl;
}

Server::~Server(){}

void Server::launch() {
	serverLoop(_endpoint);
}

const string& Server::name() const {
	return _hostName;
}

void Server::sendMessage(UserIRC* receiver, PayloadIRC payload) {
	sendMessage(NULL, receiver, payload);
}

void Server::sendMessage(UserIRC* sender, UserIRC* receiver, PayloadIRC payload) {
	MsgIRC message(sender, receiver, payload);
	_msgQueue.push(message);
}

string getDateTime() {
	time_t now = time(0);
	return ctime(&now);
}

int Server::createEndpoint(void)
{
	int result = socket(AF_INET, SOCK_STREAM, 0);
	if (result < 0)
	{
		cout << "cannot create endpoint" << endl;
		exit(0);
	}
	cout << "server socket created successfuly" << endl;
	return result;
}

void Server::bindEndpoint(void)
{
	sockaddr_in my_server;

	my_server.sin_family = AF_INET;
	my_server.sin_addr.s_addr = htons(INADDR_ANY);
	my_server.sin_port = htons(_port);
	if (bind(_endpoint, (struct sockaddr *)&my_server, sizeof(my_server)) < 0)
	{
		cout << "binding error" << endl;
		exit(0);
	}
	cout << "binding socket successfuly" << endl;
	
	listen(_endpoint, 1);
	char host[256];
  	struct hostent *host_entry;
	gethostname(host, sizeof(host));
	host_entry = gethostbyname(host);
	cout << "server ip: " << inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])) << " listening on port " << _port << " | " << _startTime;
}

void	sendToAllChan(PayloadIRC& payload, UserIRC *user,  Server &server)
{
	for (map<string, Channel>::iterator it = server._channels.begin(); it != server._channels.end(); ++it)
	{
		if ((*it).second.isInChannel(user))
			(*it).second.sendToAll(payload, server, user);
	}
}

void sendToAllChanInfo(PayloadIRC& payload, UserIRC *user, Server &server)
{
	for (map<string, Channel>::iterator it = server._channels.begin(); it != server._channels.end(); ++it)
	{
		if ((*it).second.isInChannel(user))
		{
			payload.params.push_back((*it).second._name);
			(*it).second.sendToAll(payload, server, user);
			payload.params.pop_back();
		}
	}
}

void removeUsersFromAllChans(UserIRC *user, Server &server)
{
	for (map<string, Channel>::iterator it = server._channels.begin(); it != server._channels.end(); ++it)
	{
		(*it).second.removeUsersFromChan(user);
	}
}

bool chanExist(const string& channel, Server &server)
{
	return server._channels.find(channel) != server._channels.end();
}

list<MsgIRC>::iterator otherMsgReceiver(list<MsgIRC>& Msgs, UserIRC* user)
{
	for (list<MsgIRC>::iterator it = ++Msgs.begin(); it != Msgs.end(); ++it)
	{
		if ((*it).receiver == user)
			return it;
	}
	return Msgs.end();
}

void Server::serverLoop(int &endpoint)
{
	fd_set currentSockets, availableSockets, availableWSockets;
	FD_ZERO(&currentSockets);
	FD_SET(endpoint, &currentSockets);
	string input;
	while(g_exit == false)
	{
		availableSockets = currentSockets;
		availableWSockets = currentSockets;
		if (select(1024, &availableSockets, &availableWSockets, 0, 0) < 0)
		{
			cout << "select error" << endl;
			exit(0);
		}
		for (int i = 0; i < 1024; i++)
		{
			if (FD_ISSET(i, &availableSockets))
			{
				if (i == endpoint)
				{
					UserIRC* newOne = _users.acceptNew(endpoint);
					FD_SET(newOne->fdSocket, &currentSockets);
					cout << "new connection from :" << getIPAddress(newOne) << endl;
				}
				else
				{
					list<MsgIRC> newOnes;
					if (!receiveMsg(_users.findBySocket(i), availableSockets, newOnes))
					{
						cout << "the client " << getIPAddress(_users.findBySocket(i)) << " has ctrl.c..." << endl;
						removeUsersFromAllChans(_users.findBySocket(i), *this);
						_users.removeUser(i);
						FD_CLR(i, &currentSockets);
						close(i);
					}
					while (newOnes.size())
					{
						size_t logFunction = 0;
						printPayload(newOnes.front().payload);
						if (!newOnes.front().receiver->allowed && newOnes.front().payload.command != "PASS")
						{	
							PayloadIRC payload;
							payload.command = "KILL";
							payload.trailer = "connection refused: unauthenticated";
							_msgQueue.push(MsgIRC(newOnes.front().receiver, payload));
							logFunction = 69;
						}
						else
						{
							if (_handlerFunction.find(newOnes.front().payload.command) != _handlerFunction.end())
								logFunction = _handlerFunction[newOnes.front().payload.command](newOnes.front(), *this);
							else
								cout << "the function " << newOnes.front().payload.command << " dosen't exist (yet?)" << endl;
						}
						if (logFunction == 69)
						{
							list<MsgIRC>::iterator iter;
							while ((iter = otherMsgReceiver(newOnes, newOnes.front().receiver)) != newOnes.end())
								newOnes.erase(iter);
						}
						newOnes.pop_front();
					}
				}
			}
			if (!_msgQueue.empty() && FD_ISSET(i, &availableWSockets))
			{
				if (_msgQueue.front().receiver->fdSocket == i)
				{
					sendMsg(availableWSockets, _msgQueue.front());
					if (_msgQueue.front().payload.command == "ERROR" || _msgQueue.front().payload.command == "KILL")
					{
						removeUsersFromAllChans(_msgQueue.front().receiver, *this);
						FD_CLR(_msgQueue.front().receiver->fdSocket, &currentSockets);
						close(i);
						_users.removeUser(_msgQueue.front().receiver->fdSocket);
					}
					_msgQueue.pop();
				}
			}
		}
	}
	close(endpoint);
}
