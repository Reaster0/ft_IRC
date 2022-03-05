#include "ClientList.hpp"

void ClientList::listDebug(void)
{
	cout << "list of users: " << endl;
	for (map<int, sockaddr_in>::iterator it = listClients.begin(); it != listClients.end(); ++it)
	{
		cout << "fd =" << it->first << endl;
		cout << "sin_address =" << inet_ntoa(it->second.sin_addr) << endl;
		cout << "family =" << it->second.sin_family << endl;
		cout << "port =" << ntohs(it->second.sin_port) << endl;
		cout << "zero =" << it->second.sin_zero << endl;
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

void ClientList::removeClient(const int& client)
{
	listClients.erase(client);
	close(client);
}

sockaddr_in& ClientList::operator[](const int& value)
{
	return listClients[value];
}