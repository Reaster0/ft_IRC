#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>
#include "server.hpp"

int main()
{
	int endpoint = create_server(1500);
	server_loop(endpoint);
}