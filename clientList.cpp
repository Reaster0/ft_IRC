#include "clientList.hpp"

sockaddr* clientList::newUser()
{
	listClients.push_back(sockaddr_in());
	return (sockaddr *)&listClients.back();
}

void clientList::listDebug(void)
{
	cout << "list of users: " << endl;
	for (vector<sockaddr_in>::iterator it = listClients.begin(); it != listClients.end(); ++it)
	{
		cout << (*it).sin_addr.s_addr << endl;
	}
}
int clientList::acceptNewClient(const int &endpoint)
{
	socklen_t size; //using it is probably a good idea
	return accept(endpoint, newUser(), &size);
}