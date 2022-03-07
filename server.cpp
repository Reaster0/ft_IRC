#include "server.hpp"


using namespace std;

int createEndpoint(void)
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

int createServer(const int &port_num)
{
	sockaddr_in my_server;
	int endpoint = createEndpoint();

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
	char host[256];
  	struct hostent *host_entry;
	int hostname;// maybe not usefull in the end
	hostname = gethostname(host, sizeof(host));
	host_entry = gethostbyname(host);
	cout << "server ip: " << inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])) << " listening on port " << port_num << endl;
	return endpoint;
}

#define DELETESOCKET 0

bool g_exit = false;

void sighandler(int)
{ 
	g_exit = true;
}

void serverLoop(int &endpoint)
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
					UserIRC* newOne = users.acceptNew(endpoint);
					FD_SET(newOne->fdSocket, &currentSockets);
					cout << "new connection from :" << getIPAddress(newOne) << endl;
					maxSockets++;
				}
				else
				{
					MsgIRC newOne;
					if (!receiveMsg(i, availableSockets, newOne))
					{
						cout << "the client " << getIPAddress(users.findBySocket(i)) << " has gone missing..." << endl;
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