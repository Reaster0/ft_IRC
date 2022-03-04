#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>
#include "client_list.hpp"
#include "server.cpp"

int main()
{
	int endpoint = create_server(1500);
	client_list clients;

	listen(endpoint, 1);
	while(1)
	{
		clients.accept_new_client(endpoint);
		clients.list_debug();
	}
}