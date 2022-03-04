#include "client_list.hpp"

void client_list::list_debug(void)
{
	cout << "list of users: " << endl;
	for (vector<sockaddr_in>::iterator it = list_clients.begin(); it != list_clients.end(); ++it)
	{
		cout << (*it).sin_addr.s_addr << endl;
	}
}
int client_list::accept_new_client(const int &endpoint)
{
	socklen_t size; //using it is probably a good idea
	return accept(endpoint, new_user(), &size);
}