#include "ClientList.hpp"

void ClientList::listDebug(void)
{
	cout << "list of users: " << endl;
	for (map<int, sockaddr_in>::iterator it = listClients.begin(); it != listClients.end(); ++it)
	{
		cout << (*it).first << ":" << (*it).second.sin_addr.s_addr << endl;
	}
}

int ClientList::acceptNewClient(const int &endpoint)
{
	sockaddr_in newOne;
	socklen_t size; //using it is probably a good idea
	int newFd = accept(endpoint, (sockaddr *)&newOne, &size);
	listClients.insert(make_pair(newFd, newOne));
	return newFd;
}

sockaddr_in& ClientList::operator[](const int& value)
{
	return listClients[value];
}