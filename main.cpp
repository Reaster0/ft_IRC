#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>
#include "ServerClass.hpp"
#include "Channel.hpp"

int main(int argc, char **argv)
{
	int port;
	if (argc > 2)
		return 1;
	if (argc == 2)
		port = atoi(argv[1]);
	else
		port = 6667;
	Server server(port, "bonjour");
    server.launch();
}

