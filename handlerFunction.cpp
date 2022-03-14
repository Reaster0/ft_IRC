#include "ServerClass.hpp"
#include "replies.hpp"
#include <fstream>
#define BUFFERMAX 512 //need to change accordingly

const string g_welcome[] = 
{"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⠤⠴⠒⠒⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠑⠒⠲⠦⢤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⢀⣀⡤⠒⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠒⠤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⢀⣠⠖⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠲⢄⣀⠀⠀⠀⠀⠀",
"⠀⢠⠞⠁⠀⠀⠀⠀⠀⢀⣀⡤⠤⠖⠒⠚⠛⠉⠉⠉⠉⠀⠀⠉⠉⠉⠉⠉⠙⠛⠒⠒⠶⠤⢄⣀⡀⠀⠀⠀⠀⠀⠀⠀⠙⢦⡀⠀⠀⠀",
"⠀⣿⠀⠀⠀⢀⣠⠴⠚⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠑⠲⠦⣄⡀⠀⠀⠀⠀⡇⠀⠀⠀",
"⠀⠙⠲⠶⠖⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠒⠒⠊⠁⠀⠀⠀",
"⠀⠀⠀⠀⠀⢀⡤⠶⠒⠋⠉⠉⠉⠉⠉⠓⠲⠤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⠤⠴⠶⠶⠶⠶⠤⣄⣀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⣠⠞⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠑⢦⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⠚⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠓⢦⡀⠀⠀⠀⠀",
"⠀⢠⡞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠳⡄⠀⠀⠀⠀⢀⡼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢧⡀⠀⠀",
"⠀⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡀⠀⠀⢀⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢳⡄⠀",
"⣼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣧⠀⠀⣾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢷⠀",
"⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀ft_irc⠀⠀⠀⠀⠀⠀⠀⠀⢻⠀⠀⡇⠀⠀⠀⠀⠀⠀   sucks⠀⠀⠀⠀⠀⠀⠀⠀⢸⡀",
"⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠺⡗⠂⠀⠀⠀⠀⠀⠀⠀⠀⢀⡇",
"⠸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡿⠀⠀⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀",
"⠀⠻⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡼⠁⠀⠀⠹⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡿⠀",
"⠀⠀⠹⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡜⠁⠀⠀⠀⠀⠳⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⠃⠀",
"⠀⠀⠀⠈⠳⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⠔⠋⠀⠀⠀⠀⠀⠀⠀⠘⠦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⠋⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠈⠉⠓⠲⠦⠤⠤⠤⠤⠶⠒⠉⠁⠀⠀⡀⠀⠀⠀⠀⠀⠀⢀⠀⠈⠓⠶⠤⣤⣀⣀⣀⣀⣀⣠⡤⠴⠚⠉⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⣾⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠑⠢⢄⣀⣀⡀⠀⠀⠀⠀⣀⣀⣠⠤⠂⠀⠀⠀⣇⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠔⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⢿⠀⠀⠀⠀⠀⠀⡿⠀⠀⠀⠀⠉⠙⠲⠦⠤⠤⠤⠤⠴⠚⠉⠁⠀⠀⠀⠀⠀",
"⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡇⠀⠀⠀⠀⢰⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⠤⠤⠤⠤⣄⣀⠀⠀",
"⣀⡴⠚⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣧⠀⠀⠀⠀⡾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠈⠓⢦",
"⠁⠀⠀⢀⣠⣤⣶⣿⣿⣿⣏⠉⠉⠛⣻⣶⠢⢤⣄⡀⠀⠀⠘⢦⣀⣠⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡤⠴⠒⠛⣷⣶⣤⣄⠀⠀",
"⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣿⣿⣿⡀⠀⠀⠉⣳⢤⣀⠀⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⠔⠛⣧⡀⠀⠀⢠⣿⣿⣿⣿⣿⣄",
"⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣶⣿⣿⠀⠈⠙⠲⣤⣀⣀⡀⢀⣀⣀⡤⠴⣾⡉⠀⠀⠀⢹⣿⣶⣶⣿⣿⣿⣿⣿⣿⣿",
"⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣀⣠⣾⣷⠀⠉⠉⣽⡏⠀⠀⠀⣸⣿⣦⣄⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿",
"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣤⣾⣿⣷⣄⣀⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿",
"⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠛⠛⠛⠛⠛⠛⠻⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿lpassera⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿",
"⠘⣿⣿⣿⣿⣿⣿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿earnaud⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿",
"⠀⠘⣿⣿⣿⡿⠃⠀⠀⠀⠀⠀⣠⣴⣶⣿⣿⣿⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿adenhez⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁",
"⠀⠀⠈⠻⣏⡀⠀⠀⠀⣀⠤⠾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⣿⣿⣿⡿⠋⠀⢀",
"⣦⡀⠀⠀⠈⠙⠦⣄⡞⠁⠀⠀⢻⡿⠏⠉⠉⠻⣿⣿⡿⠛⠿⣿⣿⡿⠛⠛⠻⣿⣿⠟⠉⠻⣿⡿⠋⠉⢿⡿⠃⠀⢀⣨⠿⠋⠀⠀⣴⠚",
"⠀⠉⠦⣄⠀⠀⠀⠈⠉⠲⠦⣄⣼⡁⠀⠀⠀⠀⣿⠏⠀⠀⠀⠸⡿⠁⠀⠀⠀⢹⠇⠀⠀⠀⢻⠃⠀⢀⣨⡧⠴⠒⠉⠀⠀⢀⡤⠏⠁⠀",
"⠀⠀⠀⠀⠙⠒⠢⠄⠀⠀⠀⠀⠈⠉⠉⠛⠒⠒⠛⠒⠲⠤⠤⠴⠧⠤⠤⠤⠤⠼⠖⠒⠒⠒⠛⠋⠉⠉⠁⠀⠀⣀⣀⡴⠞⠉⠀  ⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠴⠖⠒⠒⠋⠉⠀⠀⠀⠀   ⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"};

void	welcome_ascii_displayer(UserIRC* user, Server& server)
{
	PayloadIRC temp;
	int i(-1);
    while (++i < 37)
    {
     temp = PayloadIRC();
     temp.prefix = "EpikEkipEkolegram";
     temp.command = "372";
     temp.params.push_back(user->nickname);
     temp.trailer = g_welcome[i];
     server._msgQueue.push(MsgIRC(user, temp));
    }
}

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
	temp.trailer = REPLIES::RPL_MOTDSTART(server);
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

	welcome_ascii_displayer(user, server);
}

int funCap(MsgIRC& msg, Server& server)
{
	(void)msg;
	(void)server;
	return 0;
}

int NICKParser(MsgIRC& msg, Server& server)
{
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
		msg.receiver->nickname = msg.payload.params.front();
	if (msg.receiver->username != "")
		welcomeMessage(msg.receiver, server);
	return 0;
}

int USERParser(MsgIRC& msg, Server& server)
{
	// if (server._users.findByUsername(msg.payload.params.front()))
	// 	return 1;
	UserIRC* newOne = msg.receiver;
	newOne->username = msg.payload.params.front();
	newOne->realName = msg.payload.trailer;
	if (newOne->nickname != "")
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
	PayloadIRC payload;
	string origin_chan_name = msg.payload.params.front();
	if (server._channels.find(origin_chan_name) == server._channels.end())
		return 1;

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
	string chan_name =  msg.payload.params.front();
	if (server._channels.find(chan_name) == server._channels.end())
		return 1;
	Channel chan = server._channels[payload.params.front()];
	
	if (find(chan.current_users.begin(), chan.current_users.end(), msg.receiver) == chan.current_users.end())
		return 1;
	payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
	chan.sendToAll(payload, server, msg.receiver);
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

	if (!chanExist(msg.payload.params.front(), server))
	{
		payload.command = "403";
		payload.prefix = server._hostName;
		payload.params.push_back(msg.receiver->nickname);
		payload.params.push_back(msg.payload.params.front());
		payload.trailer = "No such channel";
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
		return 0;
	}
	payload.command = "353";
	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back("=");
	payload.params.push_back(msg.payload.params.front());
	payload.trailer = server._channels[msg.payload.params.front()].userList();
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload = PayloadIRC();
	payload.command = "366";
	payload.prefix = server._hostName;
	payload.params.push_back(msg.receiver->nickname);
	payload.params.push_back(msg.payload.params.front());
	payload.trailer = "End of /NAMES list.";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	return 0;
}

int MOTD(MsgIRC& msg, Server& server) {
	static const int MAX_MOTD_SIZE = 80;

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
	payload.trailer = "Alexandre, the man who love beer with a lot of alcool like a chad, Adenhez";
	server._msgQueue.push(MsgIRC(msg.receiver, payload));
	payload.trailer = "VanVan the best one, i coded this function and can say whatever i want in it, che!, (i love transexual guys and alcoholess pederastic beers) Earnaud ";
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

	if (msg.receiver->away) {
		msg.receiver->away = false;

		payload.command = REPLIES::toString(RPL_UNAWAY);
		payload.trailer = REPLIES::RPL_UNAWAY();
	} else {
		msg.receiver->away = true;

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
		payload.params.clear();
		payload.params.push_back(msg.receiver->nickname);
		payload.params.push_back(it->first);
		payload.params.push_back(to_string(it->second.current_users.size()));
		payload.trailer = it->second._topic;
		server._msgQueue.push(MsgIRC(msg.receiver, payload));
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
	if (0/*msg.receiver != operator*/)
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
		//add the nickname to a banned list
		payloaderror.command = "KILL"; //and maybe i don't send the correct packet, ho well
		payloaderror.trailer = "Kill: " + msg.payload.trailer;
		server._msgQueue.push(MsgIRC(server._users.findByNickname(msg.payload.params.front()), payloaderror));
		PayloadIRC payload;
		payload.command = "QUIT";
		payload.prefix = msg.receiver->nickname + "!" + msg.receiver->username + "@" + getIPAddress(msg.receiver);
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