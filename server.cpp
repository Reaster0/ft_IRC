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
	ClientList clients;
	fd_set currentSockets, availableSockets;
	FD_ZERO(&currentSockets);
	FD_SET(endpoint, &currentSockets);
	int maxSockets = endpoint + 1;
	while(1)
	{
		availableSockets = currentSockets;
		if (select(maxSockets, &availableSockets, 0, 0, 0) < 0)
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
					int newClient = clients.acceptNewClient(endpoint);
					FD_SET(newClient, &currentSockets);
					cout << "got a new connection from: " << inet_ntoa(clients[newClient].sin_addr) << endl;
					maxSockets++;
				}
				else
				{
					//handle the operation for current socket with client[i]
					if (DELETESOCKET)
					{
						clients.removeClient(i);
						FD_CLR(i, &currentSockets);
					}
					FD_CLR(i, &availableSockets);
				}
			}
		}
	}
}