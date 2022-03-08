#include "MsgIRC.hpp"

#define BUFFERMAX 512 //need to change accordingly

bool PayloadIRC::empty()
{
	return (command.empty() && params.empty() && prefix.empty() && trailer.empty());
}

size_t sendMsg(fd_set &availableWSockets, MsgIRC& msg)
{
	string buffer;

	if (msg.payload.empty())
		return 0;
	if (!msg.payload.prefix.empty())
		buffer += ":" + msg.payload.prefix;
	if (!msg.payload.command.empty())
		buffer += " " + msg.payload.command;
	while (!msg.payload.params.empty())
	{
		buffer += " " + msg.payload.params.front();
		msg.payload.params.pop_front();
	}
	if (!msg.payload.trailer.empty())
		buffer += " :" + msg.payload.trailer;
	buffer += "\r\n";
	FD_CLR(msg.receiver->fdSocket, &availableWSockets);
	cout << "buffer_print+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << buffer << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
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
	while ((token = strtok(NULL, " ")) && *token != ':')
	{
		payload.params.push_back(token);
	}
	if (token)
		payload.trailer = token + 1;
	token = strtok(NULL, "");
	if (token)
		payload.trailer += " " + (string)token;
}