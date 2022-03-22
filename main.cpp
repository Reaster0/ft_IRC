#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>
#include "ServerClass.hpp"
#include "Channel.hpp"

bool is_number(char *argv)
{
	int i = -1;

	while (argv[++i])
	{
		if (!isdigit(argv[i]))
			return false;
	}
	return true;
}

void	display_usage(void)
{
	cout << "usage:\n	";
	cout << "./ircserv <port> <password>" << endl; 
}

int main(int argc, char **argv)
{
	int port;
	std::string password;
	if (argc != 3)
	{
		display_usage();
		cout << "error:\n	wrong argument number" << endl;
		return 1;
	}
	if (!is_number(argv[1]))
	{
		display_usage();
		cout << "error:\n	<port> must be an integer" << endl;
		return 2;
	}
	port = atoi(argv[1]);
	password = argv[2];
	Server server(port, password);
    server.launch();
}

