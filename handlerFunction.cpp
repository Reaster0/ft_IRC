#include "ServerClass.hpp"
#include "replies.hpp"

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
	temp.command = REPLIES::toString(RPL_MOTDSTART);
	temp.params.push_back(user->nickname);
	temp.trailer = REPLIES::RPL_MOTDSTART("Le meilleur serveur");
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

int QUITParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payloaderror;
	payloaderror.command = "ERROR";
	payloaderror.trailer = (string)"Closing link " + (string)"Quit: " + msg.payload.trailer;
	server._msgQueue.push(MsgIRC(msg.receiver, payloaderror));
	
	PayloadIRC payload;
	payload.command = "PART";
	payload.trailer = msg.payload.trailer;
	if (payload.trailer == "")
		payload.trailer = "has ragequit life";
	payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
	sendToAllChanInfo(payload, msg.receiver, server);
	return 1;
}

int JOINParser(MsgIRC& msg, Server& server)
{
	if (msg.payload.params.empty())
		return 1;
	if (msg.payload.params.front()[0] != '#')
		msg.payload.params.front() = '#' + msg.payload.params.front();
	std::string chan_name = msg.payload.params.front();
	if (!server._channels.empty() && server._channels.find(chan_name) != server._channels.end() && server._channels[chan_name].isAuthorizedUser(msg.receiver) == false)
		return 2;
	if (server._channels.find(chan_name) == server._channels.end())
		server._channels[chan_name] = Channel(chan_name);
	if (server._channels[chan_name].isInChannel(msg.receiver))
		return 3;
	server._channels[chan_name].acceptUser(msg.receiver);
	// JOIN simple payload
	PayloadIRC payload;
	payload.prefix = msg.receiver->nickname + "!" +  msg.receiver->username + "@" + getIPAddress(msg.receiver);
	payload.command = "JOIN";
	payload.params.push_back(chan_name);
	server._channels[chan_name].sendToAll(payload, server);
	// 332
	payload = PayloadIRC();
	payload.command = "332";
	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back(chan_name);
	payload.trailer = server._channels[chan_name]._topic;
	MsgIRC response332(msg.receiver, payload);
	server._msgQueue.push(response332);
	// 353
	payload.params.clear();
	payload.trailer.clear();
	payload.command = "353";
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back("=");
	payload.params.push_back(chan_name);
	payload.trailer = "@";
	for(std::vector<UserIRC*>::iterator iter = server._channels[chan_name].current_users.begin(); iter != server._channels[chan_name].current_users.end(); ++iter)
	{
		payload.trailer += (*iter)->nickname;
		payload.trailer += " ";
	}
	payload.trailer.erase(payload.trailer.find_last_not_of(" ") + 1);
	MsgIRC response353(msg.receiver, payload);
	server._msgQueue.push(response353);
	// 366
	payload.command = "366";
	payload.params.clear();
	payload.trailer.clear();
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back(chan_name);
	payload.trailer = "End of NAMES list";
	MsgIRC response366(msg.receiver, payload);
	server._msgQueue.push(response366);
	// 329

	return 0;
}

int MODEParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	string origin_chan_name = msg.payload.params.front();

	payload.prefix = server._hostName;
	payload.command = "324";
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back(origin_chan_name);
	payload.params.push_back("+n");
	MsgIRC response324(msg.receiver, payload);
	server._msgQueue.push(response324);
	return 0;
}

int PRIVMSGParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload = msg.payload;
	Channel chan = server._channels[payload.params.front()];
	payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
	chan.sendToAll(payload, server, msg.receiver);
	return 0;
}

int WHOParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	Channel chan = server._channels[msg.payload.params.front()];
	// 352
	for(std::vector<UserIRC*>::iterator iter = chan.current_users.begin(); iter != chan.current_users.end(); ++iter)
    {
		
		payload.prefix = server._hostName;
		payload.command = "352";
		payload.params.push_back(msg.receiver->nickname);
		payload.params.push_back(chan._name);
		payload.params.push_back((*iter)->username);
		payload.params.push_back(getIPAddress((*iter)));
		payload.params.push_back(server._hostName);
		payload.params.push_back((*iter)->nickname);
		// last params option can change in some ways
		payload.params.push_back("H");
		MsgIRC response352(msg.receiver, payload);
		server._msgQueue.push(response352);
		payload = PayloadIRC();
		//chan.sendToAll(payload, server);
	}

	// 315
	payload = PayloadIRC();
	payload.prefix = server._hostName;
	payload.command = "315";
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back(chan._name);
	payload.trailer = "End of WHO list";
	MsgIRC response315(msg.receiver, payload);
	server._msgQueue.push(response315);

	return 0;
}
