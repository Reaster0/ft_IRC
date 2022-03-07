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

int printConnexion(const int& i)
{
	char buf[1024];
	bzero(buf, sizeof(buf));
	if (!recv(i, buf, sizeof(buf), 0))
		return 0;
	cout << buf;
	fflush(stdout);
	return 1;
}

void printPayload(PayloadIRC& payload)
{
	cout << "**************debugPayload*****************" << endl;
	cout << "command:" << payload.command << endl;
	for (list<string>::iterator it = payload.params.begin(); it != payload.params.end(); ++it)
	{
		cout << "params:" << *it << endl;
	}
	cout << "prefix:" << payload.prefix << endl;
	cout << "trailer:" << payload.trailer << endl;
	cout << "*********************************************" << endl;
}