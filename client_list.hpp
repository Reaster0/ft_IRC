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

class client_list
{
	private:
		vector<sockaddr_in> list_clients;

		sockaddr* new_user()
		{
			list_clients.push_back(sockaddr_in());
			return (sockaddr *)&list_clients.back();
		}
	public:
		client_list(){};
		~client_list(){};
		
		//print the list of users
		void list_debug(void);

		//accept a new client if there is one
		int accept_new_client(const int &endpoint);
};
