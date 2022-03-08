#include "ServerClass.hpp"

void welcomeMessage(UserIRC* user, Server& server)
{
	PayloadIRC temp;
	
	temp.prefix = "EpikEkipEkolegram";
	temp.command = "001";
	temp.params.push_back(user->nickname);
	temp.trailer = "Welcome to the Internet Relay Network " + user->nickname;
	server._msgQueue.push(MsgIRC(user, temp));
	
	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = "002";
	temp.params.push_back(user->nickname);
	temp.trailer = "Your host is EpikEkipEkolegram, running version 42.69";
	server._msgQueue.push(MsgIRC(user, temp));

	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = "003";
	temp.params.push_back(user->nickname);
	temp.trailer = "This server was created " + server._startTime;
	server._msgQueue.push(MsgIRC(user, temp));

	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = "004";
	temp.params.push_back(user->nickname);
	temp.trailer = "EpikEkipEkolegram 42.69 aiwroOs OovaimnpsrtklbeI";
	server._msgQueue.push(MsgIRC(user, temp));

	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = "375";
	temp.params.push_back(user->nickname);
	temp.trailer = "- master Message of the day - ";
	server._msgQueue.push(MsgIRC(user, temp));

	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = "372";
	temp.params.push_back(user->nickname);
	temp.trailer = "bienvenue cher tester! your nickname is:" + user->nickname;
	server._msgQueue.push(MsgIRC(user, temp));

	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = "372";
	temp.params.push_back(user->nickname);
	temp.trailer = "your username is:" + user->username;
	server._msgQueue.push(MsgIRC(user, temp));
	
	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = "372";
	temp.params.push_back(user->nickname);
	temp.trailer = "your realname is:" + user->realName;
	server._msgQueue.push(MsgIRC(user, temp));
}

int funCap(MsgIRC& msg, Server& server)
{
	(void)msg;
	(void)server;
	return 0;
}

int NICKParser(MsgIRC& msg, Server& server)
{
	if (server._users.findByNickname(msg.payload.params.front())
	|| !server._users.findFirstUnfilled())
		return 1; //probably other things too
	server._users.findFirstUnfilled()->nickname = msg.payload.params.front();
	return 0;
}

int USERParser(MsgIRC& msg, Server& server)
{
	if (server._users.findByUsername(msg.payload.params.front())
	|| !server._users.findFirstUnfilled())
		return 1;
	UserIRC* newOne = server._users.findFirstUnfilled();
	newOne->username = msg.payload.params.front();
	newOne->realName = msg.payload.trailer;
	newOne->needFill = false;
	welcomeMessage(newOne, server);
	return 0;
}

int JOINParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	if (msg.payload.params.empty())
		return 1;
	if (msg.payload.params.front()[0] != '#')
		msg.payload.params.front() = '#' + msg.payload.params.front();
	std::string chan_name = msg.payload.params.front();
	if (!server._channels.empty() && server._channels.find(chan_name) != server._channels.end() && server._channels[chan_name].isAuthorizedUser(msg.sender) == false)
		return 2;
	server._channels.insert( std::pair<string, Channel>(chan_name, Channel(chan_name)) );
	server._channels[chan_name].acceptUser(msg.sender);
	// server._channels[chan_name].getInfo();
	payload.prefix = msg.sender->nickname + "@" + getIPAddress(msg.sender);
	payload.command = "JOIN";
	payload.params.push_back(chan_name);
	// sending to all participants of the chan
	server._channels[chan_name].sendToAll(payload, server);
	return 0;
}
