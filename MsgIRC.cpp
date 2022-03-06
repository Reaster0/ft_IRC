#include "MsgIRC.hpp"

#define BUFFERMAX 4242 //need to change accordingly

size_t sendMsg(fd_set &availableWSockets, const MsgIRC& msg)
{
	FD_CLR(msg.receiver->fdSocket, &availableWSockets);
	return send(msg.receiver->fdSocket, msg.payload.c_str(), msg.payload.length(), 0);
}

size_t receiveMsg(const int& socket, fd_set &availableSockets, MsgIRC& msg)
{
	size_t result;
	char buffer[BUFFERMAX];
	bzero(buffer, BUFFERMAX);

	result = recv(socket, buffer, BUFFERMAX, 0);
	msg.payload = buffer;
	//make all of the parsing and set msg accordingly

	FD_CLR(socket, &availableSockets);
	return result;
}