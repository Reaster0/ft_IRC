#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>
#include "server.hpp"

std::string g_pwd;

int main()
{
	g_pwd = random_pwd(10);
	std::cout << "password:\n	";
	std::cout << g_pwd <<std::endl;
	int endpoint = create_server(1600);
	server_loop(endpoint);
}