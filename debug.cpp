#include "debug.hpp"

void sendToAll(fd_set& availableSockets, const int& maxSockets , const char* msg, const int& exception = 0)
{
	for (int i = 0; i < maxSockets; ++i)
	{
		if (FD_ISSET(i, &availableSockets) && i != exception)
		{
			cout << i << "is a fd valide" << endl;
			send(i, msg, strlen(msg), 0);
		}
		FD_CLR(i, &availableSockets);
	}
}

int testMessagesForAll(const int& i, fd_set& availableSockets, const int& maxSockets)
{
	char buf[1024];
	bzero(buf, sizeof(buf));
	if (!recv(i, buf, sizeof(buf), 0))
		return 0;
	sendToAll(availableSockets, maxSockets, buf, i);
	return 1;
}