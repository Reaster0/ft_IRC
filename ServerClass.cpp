#include "ServerClass.hpp"

using namespace std;

bool g_exit = false;

Server::Server() : _port(DEFAULT_PORT), _startTime(getDateTime()), _users(new UserList), _endpoint(createEndpoint()) {
	bindEndpoint();
}

Server::Server(const int& port) : _port(port), _startTime(getDateTime()), _users(new UserList), _endpoint(createEndpoint()) {
	bindEndpoint();
}

Server::~Server() {
	delete _users;
}

void Server::launch() {
	serverLoop(_endpoint);
}

string Server::getDateTime() {
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
	cout << "server ip: " << inet_ntoa(my_server.sin_addr) << " listening on port " << _port << endl;
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
					int newClient = acceptNewClient(endpoint);
					FD_SET(newClient, &currentSockets);
					cout << "new connection from :" << getIPAddress(newClient) << endl;
					maxSockets++;
				}
				else
				{
					MsgIRC newOne;
					if (!receiveMsg(i, availableSockets, newOne))
					{
						cout << "the client " << getIPAddress(i) << " has gone missing..." << endl;
						FD_CLR(i, &currentSockets);
						close(i);
					}
					//for testing purpose
					printPayload(newOne.payload);
				}
			}
			if (FD_ISSET(i, &availableWSockets))
			{
				MsgIRC AMsg;
				if (0) //if there's messages to send
				{
					sendMsg(availableWSockets, AMsg);
				}
			}
		}
	}
	close(endpoint);
}
