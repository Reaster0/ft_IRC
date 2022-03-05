#include "socket.hpp"

int acceptNewClient(const int& endpoint)
{
	sockaddr_in newOne;
	socklen_t size; //using it is probably a good idea
	return accept(endpoint, (sockaddr *)&newOne, &size);
}

char *getIPAddress(const int& socket)
{
	sockaddr_in tempAddr;
	socklen_t osef;
	getsockname(socket, (sockaddr*)&tempAddr, &osef);
	return inet_ntoa(tempAddr.sin_addr);
}