#include "ServerClass.hpp"
#include "replies.hpp"
#include <fstream>

#define BUFFERMAX 512 //need to change accordingly

void welcomeMessage(UserIRC* user, Server& server)
{
	PayloadIRC temp;
	
	temp.prefix = "EpikEkipEkolegram";
	temp.command = REPLIES::toString(RPL_WELCOME);
	temp.params.push_back(user->nickname);
	temp.trailer = REPLIES::RPL_WELCOME(user);
	server._msgQueue.push(MsgIRC(user, temp));
	
	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = REPLIES::toString(RPL_YOURHOST);
	temp.params.push_back(user->nickname);
	temp.trailer = REPLIES::RPL_YOURHOST(&server);
	server._msgQueue.push(MsgIRC(user, temp));

	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = REPLIES::toString(RPL_CREATED);
	temp.params.push_back(user->nickname);
	temp.trailer = REPLIES::RPL_CREATED(&server);
	server._msgQueue.push(MsgIRC(user, temp));

	temp = PayloadIRC();
	temp.prefix = "EpikEkipEkolegram";
	temp.command = REPLIES::toString(RPL_MYINFO);
	temp.params.push_back(user->nickname);
	temp.trailer = REPLIES::RPL_MYINFO(&server);
	server._msgQueue.push(MsgIRC(user, temp));

	MsgIRC msg(user, temp);
	MOTD(msg, server);
}

int funCap(MsgIRC& msg, Server& server)
{
	(void)msg;
	(void)server;
	return 0;
}

int NICKParser(MsgIRC& msg, Server& server)
{
	bool existingBefore = false;
	string oldNick = msg.receiver->nickname;
	if (msg.receiver->nickname != "")
		existingBefore = true;
	if (server._users.findByNickname(msg.payload.params.front()))
	{
		PayloadIRC payload;
		payload.command = "433";
		payload.prefix = server._hostName;
		payload.params.push_back("*");
		payload.params.push_back(msg.payload.params.front());
		payload.trailer = "Nickname is already in use";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
	}
	else
	{
		if (msg.receiver->nickname != "")
			server._usersHistory.push_back(*msg.receiver);
		msg.receiver->nickname = msg.payload.params.front();
		if (existingBefore)
		{
			PayloadIRC payload;
			payload.prefix = oldNick + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
			payload.command = "NICK";
			payload.trailer = msg.receiver->nickname;
			sendToAllChan2(payload, msg.receiver, server);
		}
	}
	if (msg.receiver->username != "" && !existingBefore)
		welcomeMessage(msg.receiver, server);
	return 0;
}

int USERParser(MsgIRC& msg, Server& server)
{
	bool existingBefore = false;
	if (msg.receiver->username != "")
		existingBefore = true;
	UserIRC* newOne = msg.receiver;
	newOne->username = msg.payload.params.front();
	newOne->realName = msg.payload.trailer;
	if (newOne->nickname != "" && !existingBefore)
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
	payload.command = "QUIT";
	payload.trailer = "Quit: " + msg.payload.trailer;
	payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
	sendToAllChan(payload, msg.receiver, server);
	server._usersHistory.push_back(*msg.receiver);
	return 1;
}

int	join_channel(MsgIRC& msg, Server& server, std::string chan_name)
{
	if (chan_name[0] != '#')
		chan_name = "#" + chan_name;
	if (!server._channels.empty() && server._channels.find(chan_name) != server._channels.end() && server._channels[chan_name].isAuthorizedUser(msg.receiver) == false)
		return 2;
	if (server._channels.find(chan_name) == server._channels.end())
		server._channels[chan_name] = Channel(chan_name);
	if (server._channels[chan_name].isInChannel(msg.receiver))
		return 3;
	if (server._channels[chan_name].getMode('p') && server._channels[chan_name].getMode('i') == false)
		return 4;
	if (server._channels[chan_name].getMode('i') && server._channels[chan_name].isInvited(msg.receiver) == false)
		return 5;
	if (server._channels[chan_name].getMode('l') && server._channels[chan_name].current_users.size() == static_cast<unsigned long>(server._channels[chan_name]._maximum_users))
		return 6;
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

int JOINParser(MsgIRC& msg, Server& server)
{
	if (msg.payload.params.empty())
		return 1;
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(msg.payload.params.front());
	while (getline(tokenStream, token, ','))
		tokens.push_back(token);
	for (vector<string>::iterator it=tokens.begin(); it!=tokens.end(); ++it) 
    {
		std::string chan_name = *it;
		join_channel(msg, server, chan_name);
    }
	return 0;
}

int MODEParser(MsgIRC& msg, Server& server)
{
	string& target = msg.payload.params.front();

	if (target.at(0) == '#')
		return MODEChannel(msg, server, target);
	return MODEUser(msg, server, target);
}

int MODEUser(MsgIRC& msg, Server& server, string& target) {
	PayloadIRC payload(server._hostName);
	UserIRC* user = msg.receiver;

	if (msg.payload.params.size() < 1) {
		payload.command = REPLIES::toString(ERR_NEEDMOREPARAMS);
		payload.params.push_back("MODE");
		payload.trailer = REPLIES::ERR_NEEDMOREPARAMS();
		payload.params.push_back(msg.receiver->nickname);
		server.sendMessage(msg.receiver, payload);
		return 1;
	}

	if (user->nickname != target) {
		payload.command = REPLIES::toString(ERR_USERSDONTMATCH);
		payload.trailer = REPLIES::ERR_USERSDONTMATCH();
		payload.params.push_back(msg.receiver->nickname);
		server.sendMessage(msg.receiver, payload);
		return 1;
	}

	list<string>::iterator it = msg.payload.params.begin();
	while (it != msg.payload.params.end()) {
		advance(it, 1);

		if (it != msg.payload.params.end()) {
			string& modesToApply = *it;
			char qualifier = modesToApply.at(0);


			for (string::iterator itModes = modesToApply.begin() + 1; itModes != modesToApply.end(); itModes++) {
				if (!UserModes::exist(*itModes)) {
					payload.command = REPLIES::toString(ERR_UMODEUNKNOWNFLAG);
					payload.trailer = REPLIES::ERR_UMODEUNKNOWNFLAG();
					payload.params.push_back(msg.receiver->nickname);
					server.sendMessage(msg.receiver, payload);

					payload.params.clear();
					payload.trailer.clear();
				}
			}

			for (string::iterator itModes = modesToApply.begin() + 1; itModes != modesToApply.end(); itModes++) {
				try {
					if (*itModes == MODES::AWAY) { continue; }
					if (qualifier == '-') { 
						if (*itModes != MODES::RESTRICTED)
							user->setMode(*itModes, false);
					}
					if (qualifier == '+') {
						if (*itModes != MODES::OPERATOR && *itModes != MODES::LOCAL_OPERATOR)
							user->setMode(*itModes, true); 
					}
				} catch (exception& e) {
					cout << "ERROR: " << e.what() << endl;
				}
			}
		}
	}

	payload.command = REPLIES::toString(RPL_UMODEIS);
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back(REPLIES::RPL_UMODEIS(user));

	server.sendMessage(msg.receiver, payload);
	return 0;
}

int MODEChannel(MsgIRC& msg, Server& server, string& target) {
	PayloadIRC payload(server._hostName);
	string origin_chan_name = target;
	Channel* channel;

	if (server._channels.find(origin_chan_name) == server._channels.end()) {
		payload.command = REPLIES::toString(ERR_NOSUCHCHANNEL);
		payload.params.push_back(msg.receiver->nickname);
		payload.params.push_back(origin_chan_name);
		payload.params.push_back(REPLIES::ERR_NOSUCHCHANNEL());

		server.sendMessage(msg.receiver, payload);
		return 1;
	}

	channel = &server._channels.find(origin_chan_name)->second;

	list<string>::iterator itParams = msg.payload.params.begin();
	while (itParams != msg.payload.params.end()) {
		advance(itParams, 1);

		if (itParams == msg.payload.params.end()) {
			payload.command = REPLIES::toString(RPL_CHANNELMODEIS);
			payload.params.push_back(msg.receiver->nickname);
			payload.params.push_back(channel->_name);
			payload.params.push_back(channel->getModes());

			server.sendMessage(msg.receiver, payload);
			return 1;
		}

		string& modesToApply = *itParams;
		char qualifier = modesToApply.at(0);
		int hasQualifier = (qualifier == '-' || qualifier == '+') ? 1 : 0;

		for (string::iterator it = modesToApply.begin() + hasQualifier; it != modesToApply.end(); it++) {
			if (!ChannelModes::exist(*it)) {
				payload.command = REPLIES::toString(ERR_UMODEUNKNOWNFLAG);
				payload.trailer = REPLIES::ERR_UMODEUNKNOWNFLAG();
				payload.params.push_back(msg.receiver->nickname);
				server.sendMessage(msg.receiver, payload);

				payload.params.clear();
				payload.trailer.clear();
			}
		}

		for (string::iterator it = modesToApply.begin() + hasQualifier; it != modesToApply.end(); it++) {
			if (*it == MODES::CHANNEL::USER_LIMIT_SET) {
				string* parameter = NULL;
				if (hasQualifier && qualifier == '+') {
					advance(itParams, 1);
					parameter = &(*itParams);
				}
				if (parameter) { channel->_maximum_users = atoi(parameter->c_str()); }
				channel->setMode(*it, qualifier == '+');
			} else if (hasQualifier && ChannelModes::is(*it, MODES::CHANNEL::TOGGLEABLE)) {
				channel->setMode(*it, qualifier == '+');
			} else if (ChannelModes::is(*it, MODES::CHANNEL::USER_RELATED)) {
				UserIRC* user;

				advance(itParams, 1);
				if (itParams == msg.payload.params.end()) {
					payload.command = REPLIES::toString(ERR_NEEDMOREPARAMS);
					payload.params.push_back(msg.receiver->nickname);
					payload.params.push_back("MODE");
					payload.trailer = REPLIES::ERR_NEEDMOREPARAMS();
					server.sendMessage(msg.receiver, payload);

					return 1;
				}

				user = server._users.findByNickname(*itParams);
				if (!user || !channel->isInChannel(user)) {
					payload.command = REPLIES::toString(ERR_USERNOTINCHANNEL);
					payload.params.push_back(msg.receiver->nickname);
					payload.params.push_back(*itParams);
					payload.params.push_back(channel->_name);
					payload.trailer = REPLIES::ERR_USERNOTINCHANNEL();
					server.sendMessage(msg.receiver, payload);

					return 1;
				}
				if (*it == MODES::CHANNEL::CREATOR) { continue; }
				channel->setUserMode(user, *it, qualifier == '+');
				cout << "========== INFOS ==========" << endl;
				channel->getInfo();
				cout << "= User modes: (" << channel->getUserModes(user) << ")" << endl;
				cout << "=========== END ===========" << endl;
			}
		}
	}
	payload.command = REPLIES::toString(RPL_CHANNELMODEIS);
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back(channel->_name);
	payload.params.push_back(channel->getModes());

	server.sendMessage(msg.receiver, payload);	
	return 0;
}

int dcc_send_response(MsgIRC& msg, Server& server)
{
	UserIRC *user_to_send = server._users.findByNickname(msg.payload.params.front());
	if (user_to_send == NULL)
		return 1;
	PayloadIRC payload;
	payload.command = "PRIVMSG";
	payload.prefix = msg.receiver->nickname + "!" +  getIPAddress(msg.receiver);
	payload.params.push_back(msg.payload.params.front());
	payload.trailer = msg.payload.trailer;
	payload.CTCP_Data = msg.payload.trailer.substr(1, msg.payload.trailer.size() -1);

	MsgIRC response(user_to_send, payload);
	server._msgQueue.push(response);
	return 0;
}

int dcc_resume_response(MsgIRC& msg, Server& server)
{
	(void)msg; // unused
	(void)server; // unused
	return 0;
}

int	ping_user(MsgIRC& msg, Server& server)
{
	string username = msg.payload.params.front();
	UserIRC* user = server._users.findByNickname(msg.payload.params.front());
	PayloadIRC payload;

	if (user == NULL)
	{
		payload.prefix = server._hostName;
		payload.command = "401";
		payload.params.push_back(msg.receiver->nickname);
		payload.params.push_back(username);
		payload.trailer = "No such nick";
		MsgIRC response(msg.receiver, payload);
		server._msgQueue.push(response);
	}
	else
	{
		payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
		payload.command = "PRIVMSG";
		payload.params.push_back(username);
		payload.trailer = msg.payload.trailer;
		payload.CTCP_Data = msg.payload.trailer.substr(1, msg.payload.trailer.size() - 1);
		MsgIRC response(user, payload);
		server._msgQueue.push(response);
	}
	
	return 0;
}

int PRIVMSGParser(MsgIRC& msg, Server& server)
{
	string trailer  = msg.payload.trailer;
	if (trailer.substr(0, 9) == "\001DCC SEND" && trailer.back() == '\001')
	{
		dcc_send_response(msg, server);
		return 2;
	}
	else if (trailer.substr(0, 11) == "\001DCC RESUME" && trailer.back() == '\001')
	{
		dcc_resume_response(msg, server);
		return 2;
	}
	else if (trailer.substr(0, 5) == "\001PING" && trailer.back() == '\001')
	{
		ping_user(msg, server);
		return 2;
	}
	PayloadIRC payload = msg.payload;
	string chan_name =  msg.payload.params.front();
	if (server._channels.find(chan_name) == server._channels.end())
		return 1;
	Channel chan = server._channels[payload.params.front()];
	
	if (chan.isInChannel(msg.receiver) == false && chan.getMode('n'))
		return 1;
	if (chan.getMode('m') && !msg.receiver->getMode('o') && !msg.receiver->getMode('O'))
		return 2;
	if (chan.getMode('a'))
		payload.prefix = "anonymous!anonymous@0.0.0.0";
	else
		payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
	chan.sendToAll(payload, server, msg.receiver);
	return 0;
}

int NOTICEParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload = msg.payload;
	UserIRC *user = server._users.findByNickname(msg.payload.params.front());
	if (user == NULL)
		return 1;
	payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
	MsgIRC response(user, payload);
	server._msgQueue.push(response);
	return 0;	
}

int WHOParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	string chan_name =  msg.payload.params.front();
	if (server._channels.find(chan_name) == server._channels.end())
		return 1;
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

int NAMESParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	queue<char*> chans;
	char buffer[BUFFERMAX];
	bzero(buffer, BUFFERMAX);
	strcpy(buffer, msg.payload.params.begin()->c_str());
	for (chans.push(strtok(buffer,",")); chans.back() ; chans.push(strtok(0, ",")));
	for (char *channel = chans.front(); chans.size() && chans.front(); chans.pop(),channel = chans.front())
	{
		if (!chanExist(channel, server))
		{
			payload.command = "403";
			payload.prefix = server._hostName;
			payload.params.push_back(msg.receiver->nickname);
			payload.params.push_back(msg.payload.params.front());
			payload.trailer = "No such channel";
			server._msgQueue.push(MsgIRC(msg.receiver, payload));
			return 0;
		}
		else
		{
			payload = PayloadIRC();
			payload.command = "353";
			payload.prefix = server._hostName;
			payload.params.push_back(msg.receiver->nickname);
			payload.params.push_back("=");
			payload.params.push_back(channel);
			payload.trailer = server._channels[channel].userList();
			server._msgQueue.push(MsgIRC(msg.receiver, payload));
			payload = PayloadIRC();
			payload.command = "366";
			payload.prefix = server._hostName;
			payload.params.push_back(msg.receiver->nickname);
			payload.params.push_back(msg.payload.params.front());
			payload.trailer = "End of /NAMES list.";
			server._msgQueue.push(MsgIRC(msg.receiver, payload));
		}
	}
	return 0;
}

int MOTD(MsgIRC& msg, Server& server) {
	static const int CHAR_SIZE = 1; // Unicode is 4
	static const int MAX_MOTD_SIZE = 80 * CHAR_SIZE;

	PayloadIRC payload;
	fstream file("config/motd");
	string line;

	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);
	if (!file.is_open()) {
		payload.command = REPLIES::toString(ERR_NOMOTD);
		payload.trailer = REPLIES::ERR_NOMOTD();

		server.sendMessage(msg.receiver, payload);
		return 1;
	}

	payload.command = REPLIES::toString(RPL_MOTDSTART);
	payload.trailer = REPLIES::RPL_MOTDSTART(server);
	server.sendMessage(msg.receiver, payload);

	while (getline(file, line)) {
		payload.command = REPLIES::toString(RPL_MOTD);
		
		for (size_t i = 0; i < line.length(); i += MAX_MOTD_SIZE) {
			payload.trailer = REPLIES::RPL_MOTD(line.substr(i, MAX_MOTD_SIZE));
			server.sendMessage(msg.receiver, payload);
		}
	}

	payload.command = REPLIES::toString(RPL_ENDOFMOTD);
	payload.trailer = REPLIES::RPL_ENDOFMOTD();

	server.sendMessage(msg.receiver, payload);
	return 0;
}

int TIME(MsgIRC& msg, Server& server) {
	PayloadIRC payload;

	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);

	payload.command = REPLIES::toString(RPL_TIME);
	payload.trailer = REPLIES::RPL_TIME(server);
	server.sendMessage(msg.receiver, payload);
	return 0;
}

int INFOParser(MsgIRC& msg, Server& server)
{
	if (!msg.payload.params.empty())
		return 0;
	PayloadIRC payload;
	payload.command = "371";
	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);

	payload.trailer = "BEST SERVER OF THE WORLD";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.trailer = "started in a hacking house!";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.trailer = "special thanks to:";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.trailer = "Luciano the away one, Lpassera";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.trailer = "Alexandre, the man who tartine des daronnes, Adenhez";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.trailer = "VanVan the best one, i coded this function and can say whatever i want in it, che!, (i code better than ebourdit and dreams about transexual dicks) Earnaud ";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.command = "374";
	payload.trailer = "End of INFO list";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;
}

int AWAY(MsgIRC& msg, Server& server) {
	PayloadIRC payload;

	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);
	
	if (msg.payload.trailer.empty()) {
		msg.receiver->awayMessage = "";
	} else {
		msg.receiver->awayMessage = msg.payload.trailer;
	}

	if (msg.receiver->getMode(MODES::AWAY)) {
		msg.receiver->setMode(MODES::AWAY, false);

		payload.command = REPLIES::toString(RPL_UNAWAY);
		payload.trailer = REPLIES::RPL_UNAWAY();
	} else {
		msg.receiver->setMode(MODES::AWAY, true);

		payload.command = REPLIES::toString(RPL_NOWAWAY);
		payload.trailer = REPLIES::RPL_NOWAWAY();
	}

	server.sendMessage(msg.receiver, payload);
	return 0;
}

int USERHOSTParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	payload.command = "302";
	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);
	UserIRC* user;
	for (list<string>::iterator iter = msg.payload.params.begin(); iter != msg.payload.params.end(); ++iter)
	{
		if (!payload.trailer.empty())
			payload.trailer += " ";
		user = server._users.findByNickname(*iter);
		if (user)
			payload.trailer += user->nickname + "=+" + user->username + "@" + getIPAddress(user);
	}
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;
}

int IsonParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	payload.command = "303";
	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);
	UserIRC* user;
	for (list<string>::iterator iter = msg.payload.params.begin(); iter != msg.payload.params.end(); ++iter)
	{
		user = server._users.findByNickname(*iter);
		if (!payload.trailer.empty() && user)
			payload.trailer += " ";
		if (user)
			payload.trailer += user->nickname;
	}
	payload.trailer += " ";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;
}

int PINGParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;

	payload.command = "PONG";
	payload.params.push_back(server._hostName);
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;
}

int PONGParser(MsgIRC& msg, Server& server)
{
	(void)msg; // unused
	(void)server; // unused
	return 0;
}

int PARTParser(MsgIRC& msg, Server& server)
{
	queue<char*> chans;
	char buffer[BUFFERMAX];
	bzero(buffer, BUFFERMAX);
	strcpy(buffer, msg.payload.params.begin()->c_str());
	for (chans.push(strtok(buffer,",")); chans.back() ; chans.push(strtok(0, ",")));
	for (char *channel = chans.front(); chans.size() && chans.front(); chans.pop(),channel = chans.front())
	{
		if (chanExist(channel, server))
		{
			if (server._channels[channel].isInChannel(msg.receiver))
			{
				PayloadIRC payload;
				payload.command = "PART";
				payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
				payload.params.push_back(channel);
				payload.trailer = msg.payload.trailer;
				server._channels[channel].sendToAll(payload, server);
				server._channels[channel].removeUsersFromChan(msg.receiver);
				if (server._channels[channel].current_users.empty())
					server._channels.erase(channel);
			}
			else
			{
				PayloadIRC payload;
				payload.command = "442";
				payload.prefix = server._hostName;
				payload.params.push_back(msg.receiver->nickname);
				payload.params.push_back(channel);
				payload.trailer = "You're not on that channel";
				server._msgQueue.push(MsgIRC(msg.receiver, payload));
			}
		}
		else
		{
			PayloadIRC payload;
			payload.command = "403";
			payload.prefix = server._hostName;
			payload.params.push_back(msg.receiver->nickname);
			payload.params.push_back(channel);
			payload.trailer = "No such channel";
			server._msgQueue.push(MsgIRC(msg.receiver, payload));
		}
		
	}
	return 0;
}

int TOPICParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	string chan_name =  msg.payload.params.front();
	if (server._channels.find(chan_name) == server._channels.end())
		return 1;
	Channel chan = server._channels[msg.payload.params.front()];
	if (chan.getMode('t'))
	{
		if (msg.receiver->getMode('o') == false && msg.receiver->getMode('O') == false)
			return 2;
	}
	payload.prefix = msg.receiver->nickname + "!" +  msg.receiver->username + "@" + getIPAddress(msg.receiver);
	payload.command = "TOPIC";
	payload.params.push_back(chan._name);
	payload.trailer = msg.payload.trailer;
	server._channels[msg.payload.params.front()]._topic = msg.payload.trailer;
	chan.sendToAll(payload, server);
	return 0;
}

int LISTParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;

	payload.prefix = server._hostName;
	payload.command = "322";
	for(std::map<string,Channel>::iterator it = server._channels.begin(); it != server._channels.end(); ++it) 
	{
		if (it->second.getMode('s') == false)
		{
			payload.params.clear();
			payload.params.push_back(msg.receiver->nickname);
			payload.params.push_back(it->first);
			payload.params.push_back(to_string(it->second.current_users.size()));
			payload.params.push_back(it->second.getModes());
			payload.params.push_back(it->second._topic);
			server._msgQueue.push(MsgIRC(msg.receiver, payload));
		}
	}
	payload = PayloadIRC();
	payload.prefix = server._hostName;
	payload.command = "323";
	payload.params.push_back(msg.receiver->nickname);
	payload.trailer = "End of LIST" ;
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;
}

int KICKParser(MsgIRC& msg, Server& server)
{
	if (msg.payload.params.size() != 2)
		return 0;
	queue<char*> chans;
	vector<char*> users;
	char buffer[BUFFERMAX];
	char buffer2[BUFFERMAX];
	bzero(buffer, BUFFERMAX);
	strcpy(buffer, msg.payload.params.begin()->c_str());
	for (chans.push(strtok(buffer,",")); chans.back() ; chans.push(strtok(0, ",")));
	msg.payload.params.pop_front();
	bzero(buffer2, BUFFERMAX);
	strcpy(buffer2, msg.payload.params.begin()->c_str());
	for (users.push_back(strtok(buffer2,",")); users.back() ; users.push_back(strtok(0, ",")));
	
	for (char *channel = chans.front(); chans.size() && chans.front(); chans.pop(),channel = chans.front())
	{
		if (chanExist(channel, server))
		{
			for (vector<char*>::iterator user = users.begin(); users.size() && *user; user++)
			{
				PayloadIRC payload;
				if(!server._channels[channel].isInChannel(server._users.findByNickname(*user)))
				{
					payload.command = "401";
					payload.prefix = server._hostName;
					payload.params.push_back("nickname " + (string)*user);
					payload.trailer = "No such nick";
					server._msgQueue.push(MsgIRC(msg.receiver, payload));
					continue;
				}
				payload.command = "KICK";
				payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
				payload.params.push_back(channel);
				payload.params.push_back(*user);
				payload.trailer = msg.payload.trailer;
				server._channels[channel].sendToAll(payload, server);
				server._channels[channel].removeUsersFromChan(server._users.findByNickname(*user));
			}
		}
	}
	return 0;
}

int KILLParser(MsgIRC& msg, Server& server)
{
	if (!msg.receiver->getMode(MODES::OPERATOR))
	{
		PayloadIRC payload;
		payload.command = "481";
		payload.prefix = server._hostName;
		payload.params.push_back(msg.receiver->nickname);
		payload.trailer = "Permission denied, you don't have operator privileges";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
		return 1;
	}
	if (server._users.findByNickname(msg.payload.params.front()))
	{
		PayloadIRC payloaderror;
		UserIRC* target = server._users.findByNickname(msg.payload.params.front());
		//add the nickname to a banned list
		payloaderror.command = "KILL"; //and maybe i don't send the correct packet, ho well
		payloaderror.trailer = "Kill: " + msg.payload.trailer;
		server._msgQueue.push(MsgIRC(target, payloaderror));
		PayloadIRC payload;
		payload.command = "QUIT";
		payload.prefix = target->nickname + "!" + target->username + "@" + getIPAddress(target);
		payload.trailer = "Kill: " + msg.payload.trailer;
		sendToAllChan(payload, server._users.findByNickname(msg.payload.params.front()), server);
		return 0;
	}
	PayloadIRC payload;
	payload.command = "401";
	payload.prefix = server._hostName;
	payload.params.push_back("nickname " + msg.payload.params.front());
	payload.trailer = "No such nick";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;

}

int INVITEParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;
	payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
	payload.command = "INVITE";
	for (list<string>::iterator it = msg.payload.params.begin(); it != msg.payload.params.end(); ++it)
		payload.params.push_back(*it);
	UserIRC *user_to_send = server._users.findByNickname(msg.payload.params.front());
	if (user_to_send == 0)
		return 1;
	if (user_to_send == msg.receiver)
		return 2;
	string chan_name =  msg.payload.params.back();
	if (server._channels.find(chan_name) == server._channels.end())
		return 3;
	Channel chan = server._channels[chan_name];
	if (chan.isInChannel(user_to_send))
		return 4;
	if (chan.isInvited(user_to_send))
		return 5;
	server._channels[chan_name].invited_users.push_back(user_to_send);
	server._msgQueue.push(MsgIRC(user_to_send, payload));
	payload = PayloadIRC();
	payload.prefix = server._hostName;
	payload.command = "341";
	payload.params.push_back(msg.receiver->nickname);

	for (list<string>::iterator it = msg.payload.params.begin(); it != msg.payload.params.end(); ++it)
		payload.params.push_back(*it);
	server._msgQueue.push(MsgIRC(msg.receiver, payload));

	//server._channels[chan_name].getInfo();
	return 0;
}

int WHOISParser(MsgIRC& msg, Server& server)
{
	queue<char*> users;
	char buffer[BUFFERMAX];
	bzero(buffer, BUFFERMAX);
	strcpy(buffer, msg.payload.params.back().c_str());
	for (users.push(strtok(buffer,",")); users.back() ; users.push(strtok(0, ",")));

	for (char* user = users.front(); users.size() && user; users.pop(), user = users.front())
	{
		PayloadIRC payload;
		UserIRC* target = server._users.findByNickname(user);
		payload.prefix = server._hostName;
		payload.params.push_back(msg.receiver->nickname);
		payload.params.push_back(user);

		if (!target)
		{
			payload.trailer = "No such nick";
			payload.command = "401";
			server._msgQueue.push(MsgIRC(msg.receiver, payload));
			payload.command = "318";
			payload.trailer = "End of /WHOIS list.";
			server._msgQueue.push(MsgIRC(msg.receiver, payload));
			continue;
		}
		
		payload.params.push_back(target->username);
		payload.params.push_back(getIPAddress(target));
		payload.params.push_back("*");
		payload.trailer = target->realName;
		payload.command = "311";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));

		payload.params.pop_back();
		payload.params.pop_back();
		payload.params.pop_back();
		payload.trailer.clear();
		for (map<string, Channel>::iterator iter = server._channels.begin(); iter != server._channels.end(); ++iter)
		{
			if ((*iter).second.isInChannel(target))
			{
				if (!payload.trailer.empty())
					payload.trailer += " ";
				payload.trailer += "@" + (*iter).first;
			}
		}
		payload.command = "319";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));

		payload.params.push_back(server._hostName);
		payload.trailer = "the best irc server";
		payload.command = "312";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));

		payload.params.pop_back();
		payload.params.push_back(to_string(difftime(time(0), target->idle)));
		payload.params.push_back(to_string(target->idle));
		payload.trailer = "seconds idle";
		payload.command = "317";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));

		payload.params.pop_back();
		payload.params.pop_back();
		payload.trailer = "End of /WHOIS list.";
		payload.command = "318";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
	}
	return 1;
}

int WHOWASParser(MsgIRC& msg, Server& server)
{
	queue<char*> users;
	char buffer[BUFFERMAX];
	bzero(buffer, BUFFERMAX);
	strcpy(buffer, msg.payload.params.front().c_str());
	for (users.push(strtok(buffer,",")); users.back(); users.push(strtok(0, ",")));
	list<UserIRC> history = server._usersHistory;

	for (char* user = users.front(); users.size() && user; users.pop(), user = users.front())
	{
		PayloadIRC payload;
		UserIRC* target = 0;
		for (list<UserIRC>::iterator it = history.begin(); it != history.end(); ++it)
			if ((*it).nickname == user)
			{
				target = &*it;
				history.erase(it);
				break;
			}
		if (!target)
		{
			payload.prefix = server._hostName;
			payload.command = "406";
			payload.params.push_back(msg.receiver->nickname);
			payload.params.push_back(user);
			payload.trailer = "There was no such nickname";
			server._msgQueue.push(MsgIRC(msg.receiver,payload));
			payload.trailer = "End of WHOAS";
			payload.command = "369";
			server._msgQueue.push(MsgIRC(msg.receiver,payload));
			continue;
		}
		payload.prefix = server._hostName;
		payload.command = "314";
		payload.params.push_back(msg.receiver->nickname);
		payload.params.push_back(target->nickname);
		payload.params.push_back(target->username);
		payload.params.push_back(target->ip);
		payload.params.push_back("*");
		payload.trailer = target->realName;
		server._msgQueue.push(MsgIRC(msg.receiver, payload));

		payload.params.pop_back();
		payload.params.pop_back();
		payload.params.push_back(server._hostName);
		payload.trailer = getDateTime();
		payload.command = "312";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));

		payload.params.pop_back();
		payload.trailer = "End of WHOWAS";
		payload.command = "369";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
	}
	return 0;
}

int ADMINParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;

	payload.prefix = server._hostName;
	payload.command = "256";
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back(server._hostName);
	payload.trailer = "Administrative Info";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));

	payload.params.pop_back();
	payload.command = "257";
	payload.trailer = "Name: LinusTechTips";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.command = "258";
	payload.trailer = "Nickname: LTT";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.command = "259";
	payload.trailer = "Email: info@linusmediagroup.com";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;
}

int LUSERSParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;

	payload.prefix = server._hostName;
	payload.command = "251";
	payload.params.push_back(msg.receiver->nickname);
	payload.trailer = "There are " + to_string(server._users.size()) + " users and 0 invisible on 1 servers";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));

	payload.command = "254";
	payload.params.push_back(to_string(server._channels.size()));
	payload.trailer = "channels formed";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));

	payload.params.pop_back();
	payload.command = "255";
	payload.trailer = "I have " + to_string(server._users.size()) + " clients and 1 servers";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;
}

int OPERATORParser(MsgIRC& msg, Server& server)
{
	PayloadIRC payload;

	if (msg.payload.params.size() != 2)
	{
		payload.prefix = server._hostName;
		payload.command = "461";
		payload.params.push_back(msg.receiver->nickname);
		payload.params.push_back("OPER");
		payload.trailer = "Parameters incorrect";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
	}
	else if	(msg.payload.params.front() != "macron")
	{
		payload.prefix = server._hostName;
		payload.command = "491";
		payload.params.push_back(msg.receiver->nickname);
		payload.trailer = "No O-lines for your host";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
	}
	else if (msg.payload.params.back() != "demission")
	{
		payload.prefix = server._hostName;
		payload.command = "464";
		payload.params.push_back(msg.receiver->nickname);
		payload.trailer = "Password incorrect";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
	}
	else
	{
		msg.receiver->setMode(MODES::OPERATOR, true);
		payload.prefix = server._hostName;
		payload.command = "381";
		payload.params.push_back(msg.receiver->nickname);
		payload.trailer = "You are an IRC operator (good answer btw)";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
		for (map<string, Channel>::iterator iter = server._channels.begin(); iter != server._channels.end(); ++iter)
		{
			if ((*iter).second.isInChannel(msg.receiver))
				(*iter).second.setUserMode(msg.receiver, 'o', true);
		}
	}
	return 0;
}

int PASSParser(MsgIRC& msg, Server& server)
{
	if (msg.payload.params.front() != server._password)
	{
		PayloadIRC payload;
		payload.command = "KILL";
		payload.trailer = "connection refused: wrong password";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
		return 69;
	}
	msg.receiver->allowed = true;
	return 0;
}
