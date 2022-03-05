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

void server_loop(int &endpoint)
{
	fd_set currentSockets, availableSockets, availableTest;
	FD_ZERO(&currentSockets);
	FD_SET(endpoint, &currentSockets);
	int maxSockets = endpoint + 1;
	string input;
	struct timeval tv = {3, 0};
	
	while(1)
	{
		availableSockets = currentSockets;
		availableTest = currentSockets;
		if (select(maxSockets, &availableSockets, &availableTest, 0, 0) < 0)
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
					//handle the operation for current socket with client[i]
					testMessagesForAll(i, availableTest, maxSockets);
					
					if (DELETESOCKET)
					{
						FD_CLR(i, &currentSockets);
					}
					FD_CLR(i, &availableSockets);
				}
			}
		}
	}
}