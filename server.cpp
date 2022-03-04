#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

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

//input the port number of the server and return the fd of the server socket
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
	return endpoint;
}