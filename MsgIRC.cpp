#include "MsgIRC.hpp"

#define BUFFERMAX 512 //need to change accordingly

size_t sendMsg(fd_set &availableWSockets, MsgIRC& msg)
{
	string buffer;
	if (msg.payload.command != "")
		buffer += msg.payload.command;
	for (vector<string>::iterator it = msg.payload.params.begin(); it != msg.payload.params.end(); ++it)
	{
		buffer += *it;
	}
	if (msg.payload.prefix != "")
		buffer += msg.payload.prefix;
	if (msg.payload.trailer != "")
		buffer += msg.payload.trailer;
	FD_CLR(msg.receiver->fdSocket, &availableWSockets);
	return send(msg.receiver->fdSocket, buffer.c_str(), buffer.length(), 0);
}

size_t receiveMsg(const int& socket, fd_set &availableSockets, MsgIRC& msg)
{
	size_t result;
	char buffer[BUFFERMAX];
	bzero(buffer, BUFFERMAX);

	result = recv(socket, buffer, BUFFERMAX, 0);
	parsingToPayload(buffer, msg.payload);
	//make all of the parsing and set msg accordingly

	FD_CLR(socket, &availableSockets);
	return result;
}

void parsingToPayload(char* buffer, PayloadIRC& payload)
{
	char *token;
	if (!*buffer)
		return;
	if (*buffer == ':')
	{
		token = strtok(buffer, " \r\n");
		payload.prefix = token;
		token = strtok(0, " \r\n");
	}
	else
		token = strtok(buffer, " \r\n");
	if (token)
		payload.command = token;
	while ((token = strtok(NULL, " \r\n")) && *token != ':')
	{
		payload.params.push_back(token);
	}
	if (token)
		payload.trailer = token + 1;
	token = strtok(NULL, "\r\n");
	if (token)
		payload.trailer += " " + (string)token;
}