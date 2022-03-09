#include "ServerClass.hpp"

using namespace std;

bool g_exit = false;

void Server::initializeMap()
{
	_handlerFunction["CAP"] = funCap;
	_handlerFunction["NICK"] = NICKParser;
	_handlerFunction["USER"] = USERParser;
	_handlerFunction["QUIT"] = QUITParser;
	_handlerFunction["JOIN"] = JOINParser;
}

Server::Server() : _port(DEFAULT_PORT), _startTime(getDateTime()), _endpoint(createEndpoint()) {
	initializeMap();
	bindEndpoint();
}

Server::Server(const int& port) : _port(port), _startTime(getDateTime()), _endpoint(createEndpoint()) {
	initializeMap();
	bindEndpoint();
}

Server::~Server() { }

void Server::launch() {
	serverLoop(_endpoint);
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
	int hostname;// maybe not usefull in the end
	hostname = gethostname(host, sizeof(host));
	host_entry = gethostbyname(host);
	time_t now = time(0);
	cout << "server ip: " << inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])) << " listening on port " << _port << " | " << _startTime <<endl;
}

void	sendToAllChan(PayloadIRC& payload, UserIRC *user,  Server &server)
{
	for (map<string, Channel>::iterator it = server._channels.begin(); it != server._channels.end(); ++it)
	{
		if ((*it).second.isInChannel(user))
			(*it).second.sendToAll(payload, server);
	}
}

void removeUsersFromAllChans(UserIRC *user, Server &server)
{
	for (map<string, Channel>::iterator it = server._channels.begin(); it != server._channels.end(); ++it)
	{
		(*it).second.removeUsersFromChan(user);
	}
}

void Server::serverLoop(int &endpoint)
{
	fd_set currentSockets, availableSockets, availableWSockets;
	FD_ZERO(&currentSockets);
	FD_SET(endpoint, &currentSockets);
	int maxSockets = endpoint + 1;
	string input;
	char buffer[BUFFER_SIZE];
	while(g_exit == false)
	{
		availableSockets = currentSockets;
		availableWSockets = currentSockets;
		if (select(maxSockets, &availableSockets, &availableWSockets, 0, 0) < 0)
		{
			cout << "select error" << endl;
			exit(0);
		}
		for (int i = 0; i < maxSockets; i++)
		{
			if (FD_ISSET(i, &availableSockets))
			{
				if (i == endpoint)
				{
					UserIRC* newOne = _users.acceptNew(endpoint);
					FD_SET(newOne->fdSocket, &currentSockets);
					cout << "new connection from :" << getIPAddress(newOne) << endl;
					maxSockets++;
				}
				else
				{
					queue<MsgIRC> newOnes;
					if (!receiveMsg(_users.findBySocket(i), availableSockets, newOnes))
					{
						cout << "the client " << getIPAddress(_users.findBySocket(i)) << " has gone missing..." << endl;
						if (_users.findBySocket(i))
						{
							_users.removeUser(i);
							removeUsersFromAllChans(_users.findBySocket(i), *this);
						}
						FD_CLR(i, &currentSockets);
						close(i);
					}
					while (newOnes.size())
					{
						printPayload(newOnes.front().payload);
						if (_handlerFunction.find(newOnes.front().payload.command) != _handlerFunction.end())
							_handlerFunction[newOnes.front().payload.command](newOnes.front(), *this);
						else
							cout << "the function " << newOnes.front().payload.command << " dosen't exist (yet?)" << endl;
						newOnes.pop();
					}
				}
			}
			if (!_msgQueue.empty() && FD_ISSET(i, &availableWSockets) && _msgQueue.front().receiver->fdSocket == i)
			{
				cout << "i send the packet:";
				printPayload(_msgQueue.front().payload);
				sendMsg(availableWSockets, _msgQueue.front());
				_msgQueue.pop();
			}
		}
	}
	close(endpoint);
}