#include "socket.hpp"

int acceptNewClient(const int& endpoint, sockaddr_in* addr, socklen_t* len_s)
{
	if (addr && len_s)
		return accept(endpoint, (sockaddr*)addr, len_s);
	sockaddr_in newOne;
	socklen_t size; //using it is probably a good idea
	return accept(endpoint, (sockaddr *)&newOne, &size);
}

string getIPAddress(const UserIRC* user)
{
	if (!user)
		return "";
	char buffer[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &user->addr.sin_addr.s_addr, buffer, INET_ADDRSTRLEN);
	return buffer;
}