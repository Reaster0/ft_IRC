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

void welcomeMessage(UserIRC* user, queue<MsgIRC>& msgQueue)
{
	PayloadIRC test;

	test.prefix = "Reaster.lolLand.fr";
	test.command = "001";
	test.params.push_back(user->nickname);
	test.trailer = "Welcome to the Internet Relay Network " + user->nickname;
	msgQueue.push(MsgIRC(user, test));
	
	test = PayloadIRC();
	test.prefix = "Reaster.lolLand.fr";
	test.command = "002";
	test.params.push_back(user->nickname);
	test.trailer = "Your host is Reaster.lolLand.fr, running version 1.0";
	msgQueue.push(MsgIRC(user, test));

	test = PayloadIRC();
	test.prefix = "Reaster.lolLand.fr";
	test.command = "003";
	test.params.push_back(user->nickname);
	test.trailer = "This server was created Mon Feb 21 17:32:09 2022";
	msgQueue.push(MsgIRC(user, test));

	test = PayloadIRC();
	test.prefix = "Reaster.lolLand.fr";
	test.command = "004";
	test.params.push_back(user->nickname);
	test.trailer = "master.ircgod.com 1.0 aiwroOs OovaimnpsrtklbeI";
	msgQueue.push(MsgIRC(user, test));

	test = PayloadIRC();
	test.prefix = "Reaster.lolLand.fr";
	test.command = "375";
	test.params.push_back(user->nickname);
	test.trailer = "- master.ircgod.com Message of the day - ";
	msgQueue.push(MsgIRC(user, test));

	test = PayloadIRC();
	test.prefix = "Reaster.lolLand.fr";
	test.command = "372";
	test.params.push_back(user->nickname);
	test.trailer = "bienvenue cher tester! your nickname is:" + user->nickname;
	msgQueue.push(MsgIRC(user, test));
}

void NICKParser(const PayloadIRC& payload, UserList& users)
{
	users.findFirstUnfilled()->nickname = payload.params.front();
}

void USERParser(const PayloadIRC& payload, UserList& users)
{
	UserIRC* newOne = users.findFirstUnfilled();
	newOne->username = payload.params.front();
	newOne->realName = payload.trailer;
	newOne->needFill = false;
}

void bigParser(const PayloadIRC& payload, queue<MsgIRC>& msgQueue, UserList& users)
{
	if (payload.command == "CAP")
		return;
	if (payload.command == "NICK")
		NICKParser(payload, users);
	if (payload.command == "USER")
	{
		USERParser(payload, users);
		welcomeMessage(users.findByUsername(payload.params.front()), msgQueue);
	}
}