#include "server.hpp"


using namespace std;

int create_endpoint(void)
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

int create_server(const int &port_num)
{
	sockaddr_in my_server;
	int endpoint = create_endpoint();

	my_server.sin_family = AF_INET;
	my_server.sin_addr.s_addr = htons(INADDR_ANY);
	my_server.sin_port = htons(port_num);
	if (bind(endpoint, (struct sockaddr *)&my_server, sizeof(my_server)) < 0)
	{
		cout << "binding error" << endl;
		exit(0);
	}
	cout << "binding socket successfuly" << endl;
	listen(endpoint, 1);
	cout << "server ip: " << inet_ntoa(my_server.sin_addr) << " listening on port " << port_num << endl;
	return endpoint;
}

#define DELETESOCKET 0

bool g_exit = false;

void sighandler(int)
{ 
	g_exit = true;
}

void server_loop(int &endpoint)
{
	fd_set currentSockets, availableSockets, availableWSockets;
	FD_ZERO(&currentSockets);
	FD_SET(endpoint, &currentSockets);
	int maxSockets = endpoint + 1;
	string input;
	UserList users;
	queue<MsgIRC> msgQueue;

	int bufsize = 1024;
	char buffer[bufsize];

	signal(SIGQUIT, sighandler);
	signal(SIGINT, sighandler);
		
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
					int userSocket = users.acceptNew(endpoint);
					FD_SET(userSocket, &currentSockets);
					cout << "new connection from :" << getIPAddress(userSocket) << endl;
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
					printPayload(newOne.payload);
					bigParser(newOne.payload, msgQueue, users);
					//need to process the payload
				}
			}
			if (!msgQueue.empty() && FD_ISSET(i, &availableWSockets) && msgQueue.front().receiver->fdSocket == i)
			{
				cout << "i send the packet:";
				printPayload(msgQueue.front().payload);
				sendMsg(availableWSockets, msgQueue.front());
				msgQueue.pop();
			}
		}
	}
	close(endpoint);
}