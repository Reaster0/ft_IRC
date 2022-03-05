#ifndef CLIENTLIST
#define CLIENTLIST

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class ClientList
{
	private:
		map<int, sockaddr_in> listClients;

	public:
		ClientList(){};
		~ClientList(){};
		
		//print the list of users
		void listDebug(void);

		//accept a new client if there is one
		int acceptNewClient(const int &endpoint);

		sockaddr_in& operator[](const int& value);
};

#endif