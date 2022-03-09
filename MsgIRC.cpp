#include "MsgIRC.hpp"

#define BUFFERMAX 512 //need to change accordingly

MsgIRC::MsgIRC()
{
	UserIRC nul_usr;
	sockaddr_in null_sockaddr;

	nul_usr.fdSocket = 0;
	nul_usr.username = "nul_user";
	nul_usr.nickname = "nul_user";
	nul_usr.addr = null_sockaddr;
	sender = &nul_usr;
	receiver = &nul_usr;
}

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
	// cout << "buffer_print+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << buffer;
	// cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	return send(msg.receiver->fdSocket, buffer.c_str(), buffer.length(), 0);
}

size_t receiveMsg(UserIRC* user, fd_set &availableSockets, queue<MsgIRC>& msg)
{
	size_t result;
	char buffer[BUFFERMAX];
	queue<char*> buf;
	char *temp;
	bzero(buffer, BUFFERMAX);

	result = recv(user->fdSocket, buffer, BUFFERMAX, 0);
	temp = strtok(buffer, "\r\n");
	do
	{
		buf.push(temp);
		temp = strtok(0, "\r\n");
	} while(temp);
	while (!buf.empty())
	{
		msg.push(MsgIRC(user, parsingToPayload(buf.front())));
		buf.pop();
	}
	FD_CLR(user->fdSocket, &availableSockets);
	return result;
}

PayloadIRC parsingToPayload(char* buffer)
{
	PayloadIRC payload;

	char *token;
	if (!buffer || !*buffer)
		return payload;
	if (*buffer == ':')
	{
		token = strtok(buffer, " ");
		payload.prefix = token;
		token = strtok(0, " ");
	}
	else
		token = strtok(buffer, " ");
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
	return payload;
}