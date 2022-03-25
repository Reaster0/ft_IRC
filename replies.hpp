#ifndef REPLIES_HPP
#define REPLIES_HPP
#include <string>
#include <sstream>
#include "ServerClass.hpp"

using namespace std;
enum numeric_replies_t {
	RPL_WELCOME 			= 001,
	RPL_YOURHOST 			= 002,
	RPL_CREATED 			= 003,
	RPL_MYINFO 				= 004,
	RPL_BOUNCE 				= 005,
	RPL_USERHOST 			= 302,
	RPL_ISON 				= 303,
	RPL_AWAY 				= 301,
	RPL_UNAWAY 				= 305,
	RPL_NOWAWAY 			= 306,
	RPL_WHOISUSER 			= 311,
	RPL_WHOISSERVER 		= 312,
	RPL_WHOISOPERATOR 		= 313,
	RPL_WHOISIDLE 			= 317,
	RPL_ENDOFWHOIS 			= 318,
	RPL_WHOISCHANNELS 		= 319,
	RPL_WHOWASUSER 			= 314,
	RPL_ENDOFWHOWAS 		= 369,
	RPL_LISTSTART 			= 321,
	RPL_LIST 				= 322,
	RPL_LISTEND 			= 323,
	RPL_UNIQOPIS 			= 325,
	RPL_CHANNELMODEIS 		= 324,
	RPL_NOTOPIC 			= 331,
	RPL_TOPIC 				= 332,
	RPL_INVITING 			= 341,
	RPL_SUMMONING 			= 342,
	RPL_INVITELIST 			= 346,
	RPL_ENDOFINVITELIST 	= 347,
	RPL_EXCEPTLIST 			= 348,
	RPL_ENDOFEXCEPTLIST 	= 349,
	RPL_VERSION 			= 351,
	RPL_WHOREPLY 			= 352,
	RPL_ENDOFWHO 			= 315,
	RPL_NAMREPLY 			= 353,
	RPL_ENDOFNAMES 			= 366,
	RPL_LINKS 				= 364,
	RPL_ENDOFLINKS 			= 365,
	RPL_BANLIST 			= 367,
	RPL_ENDOFBANLIST 		= 368,
	RPL_INFO 				= 371,
	RPL_ENDOFINFO 			= 374,
	RPL_MOTDSTART 			= 375,
	RPL_MOTD 				= 372,
	RPL_ENDOFMOTD 			= 376,
	RPL_YOUREOPER 			= 381,
	RPL_REHASHING 			= 382,
	RPL_YOURESERVICE 		= 383,
	RPL_TIME 				= 391,
	RPL_USERSSTART 			= 392,
	RPL_USERS 				= 393,
	RPL_ENDOFUSERS 			= 394,
	RPL_NOUSERS 			= 395,
	RPL_TRACELINK 			= 200,
	RPL_TRACECONNECTING		= 201,
	RPL_TRACEHANDSHAKE 		= 202,
	RPL_TRACEUNKNOWN 		= 203,
	RPL_TRACEOPERATOR 		= 204,
	RPL_TRACEUSER 			= 205,
	RPL_TRACESERVER 		= 206,
	RPL_TRACESERVICE 		= 207,
	RPL_TRACENEWTYPE 		= 208,
	RPL_TRACECLASS 			= 209,
	RPL_TRACERECONNECT 		= 210,
	RPL_TRACELOG 			= 261,
	RPL_TRACEEND 			= 262,
	RPL_STATSLINKINFO 		= 211,
	RPL_STATSCOMMANDS 		= 212,
	RPL_ENDOFSTATS 			= 219,
	RPL_STATSUPTIME 		= 242,
	RPL_STATSOLINE 			= 243,
	RPL_UMODEIS 			= 221,
	RPL_SERVLIST 			= 234,
	RPL_SERVLISTEND 		= 235,
	RPL_LUSERCLIENT 		= 251,
	RPL_LUSEROP 			= 252,
	RPL_LUSERUNKNOWN 		= 253,
	RPL_LUSERCHANNELS 		= 254,
	RPL_LUSERME 			= 255,
	RPL_ADMINME 			= 256,
	RPL_ADMINLOC1 			= 257,
	RPL_ADMINLOC2 			= 258,
	RPL_ADMINEMAIL 			= 259,
	RPL_TRYAGAIN 			= 263,
	ERR_NOSUCHNICK 			= 401,
	ERR_NOSUCHSERVER 		= 402,
	ERR_NOSUCHCHANNEL 		= 403,
	ERR_CANNOTSENDTOCHAN 	= 404,
	ERR_TOOMANYCHANNELS 	= 405,
	ERR_WASNOSUCHNICK 		= 406,
	ERR_TOOMANYTARGETS 		= 407,
	ERR_NOSUCHSERVICE 		= 408,
	ERR_NOORIGIN 			= 409,
	ERR_NORECIPIENT 		= 411,
	ERR_NOTEXTTOSEND 		= 412,
	ERR_NOTOPLEVEL 			= 413,
	ERR_WILDTOPLEVEL 		= 414,
	ERR_BADMASK 			= 415,
	ERR_UNKNOWNCOMMAND 		= 421,
	ERR_NOMOTD 				= 422,
	ERR_NOADMININFO 		= 423,
	ERR_FILEERROR 			= 424,
	ERR_NONICKNAMEGIVEN 	= 431,
	ERR_ERRONEUSNICKNAME 	= 432,
	ERR_NICKNAMEINUSE 		= 433,
	ERR_NICKCOLLISION 		= 436,
	ERR_UNAVAILRESOURCE 	= 437,
	ERR_USERNOTINCHANNEL 	= 441,
	ERR_NOTONCHANNEL 		= 442,
	ERR_USERONCHANNEL 		= 443,
	ERR_NOLOGIN 			= 444,
	ERR_SUMMONDISABLED 		= 445,
	ERR_USERSDISABLED 		= 446,
	ERR_NOTREGISTERED 		= 451,
	ERR_NEEDMOREPARAMS 		= 461,
	ERR_ALREADYREGISTRED 	= 462,
	ERR_NOPERMFORHOST 		= 463,
	ERR_PASSWDMISMATCH 		= 464,
	ERR_YOUREBANNEDCREEP 	= 465,
	ERR_YOUWILLBEBANNED 	= 466,
	ERR_KEYSET 				= 467,
	ERR_CHANNELISFULL 		= 471,
	ERR_UNKNOWNMODE 		= 472,
	ERR_INVITEONLYCHAN 		= 473,
	ERR_BANNEDFROMCHAN 		= 474,
	ERR_BADCHANNELKEY 		= 475,
	ERR_BADCHANMASK 		= 476,
	ERR_NOCHANMODES 		= 477,
	ERR_BANLISTFULL 		= 478,
	ERR_NOPRIVILEGES 		= 481,
	ERR_CHANOPRIVSNEEDED 	= 482,
	ERR_CANTKILLSERVER 		= 483,
	ERR_RESTRICTED 			= 484,
	ERR_UNIQOPPRIVSNEEDED 	= 485,
	ERR_NOOPERHOST 			= 491,
	ERR_UMODEUNKNOWNFLAG 	= 501,
	ERR_USERSDONTMATCH 		= 502,
};

namespace REPLIES {
	// ---- Replies
	string RPL_WELCOME(UserIRC* user) {return "Welcome to the Internet Relay Network " + user->nickname + "!" + user->username + "@" + user->ip; }
	string RPL_YOURHOST(Server* server) { return "Your host is " + server->_hostName + ", running version " + server->_version; }
	string RPL_CREATED(Server* server) { return "This server was created" + server->_startTime; }
	string RPL_MYINFO(Server* server) { return server->_hostName + " " + server->_version + " " + MODES::USER_ALL + " " + MODES::CHANNEL::ALL + MODES::CHANNEL::USER_RELATED; }
	string RPL_BOUNCE(string server, string port) { return "Try server " + server + ", port " + port; }

	string RPL_USERHOST(string replies) { return ":" + replies; }
	string RPL_ISON(string nicknames) { return ":" + nicknames; }
	string RPL_AWAY(string nickname, string awayMessage) { return nickname + " :" + awayMessage; }
	string RPL_UNAWAY(void) { return ":You are no longer marked as being away"; }
	string RPL_NOWAWAY(void) { return ":You have been marked as being away"; }
	string RPL_WHOISUSER(string nickname, string user, string host, string realName) { return nickname + " " + user + " " + host + " * :" + realName; }
	string RPL_WHOISSERVER(string nickname, string server, string serverInfo) { return nickname + " " + server + " :" + serverInfo; }
	string RPL_WHOISOPERATOR(string nickname) { return nickname + " :is an IRC operator"; }
	string RPL_WHOISIDLE(string nickname, string seconds) { return nickname + " " + seconds + ": seconds idle"; }
	string RPL_ENDOFWHOIS(string nickname) { return nickname + ":End of WHOIS list"; }
	string RPL_WHOISCHANNELS(string nickname, string channelsInfos) { return nickname + " :" + channelsInfos; }
	string RPL_WHOWASUSER(string nickname, string user, string host, string realName) { return nickname + " " + user + " " + host + " * :" + realName; }
	string RPL_ENDOFWHOWAS(string nickname) { return nickname + ":End of WHOWAS"; }
	string RPL_LISTSTART(void) { return ""; } // OBSOLETE Not used

	string RPL_LIST(string channel, string visibleUsersCount, string topic) { return channel + " " + visibleUsersCount + ":" + topic; }
	string RPL_LISTEND(void) { return ":End of LIST"; }
	string RPL_UNIQOPIS(string channel, string nickname) { return channel + " " + nickname; }
	string RPL_CHANNELMODEIS(Channel& channel, string modeParams) { return channel._name + " " + channel.getModes() + " " + modeParams; }
	string RPL_NOTOPIC(string channel) { return channel + " :No topic is set"; }
	string RPL_TOPIC(string channel, string topic) { return channel + " :" + topic; }
	string RPL_INVITING(string channel, string nickname) { return channel + " " + nickname; }
	string RPL_SUMMONING(string user) { return user + " :Summoning user to IRC"; }
	string RPL_INVITELIST(string channel, string inviteMask) { return channel + " " + inviteMask; }
	string RPL_ENDOFINVITELIST(string channel) { return channel + " :End of channel invite list"; }
	string RPL_EXCEPTLIST(string channel, string exceptionMask) { return channel + " " + exceptionMask; }
	string RPL_ENDOFEXCEPTLIST(string channel ) { return channel + " :End of channel exception list"; }

	string RPL_VERSION(string version, string debugLevel, string server, string comments) { return version + "." + debugLevel + " " + server + " :" + comments; }
	string RPL_WHOREPLY(string channel, string user, string host, string server, string nickname, string hopCount, string realName) { return channel + " " + user + " " + host + " " + server + " " + nickname + " :" + hopCount + " " + realName; }
	string RPL_ENDOFWHO(string name) { return name + " :End of WHO list"; }
	string RPL_NAMREPLY(string channelWithType, string nicknames) { return channelWithType + " " + nicknames; }
	string RPL_ENDOFNAMES(string channel) { return channel + " :End of NAMES list"; }
	string RPL_LINKS(string mask, string server, string hopCount, string serverInfo) { return mask + " " + server + " :" + hopCount + " " + serverInfo; }
	string RPL_ENDOFLINKS(string mask) { return mask + " :End of LINKS list"; }
	string RPL_BANLIST(string channel, string banmask) { return channel + " " + banmask; }
	string RPL_ENDOFBANLIST(string channel) { return channel + " :End of channel ban list"; }

	string RPL_INFO(string info) { return ":" + info; }
	string RPL_ENDOFINFO(void) { return ":End of INFO list"; }

	string RPL_MOTDSTART(Server& server) { return ":- " + server.name() + " Message of the day - "; }
	string RPL_MOTD(string text) { return ":- " + text; }
	string RPL_ENDOFMOTD(void) { return ":End of MOTD command"; }

	string RPL_YOUREOPER(void) { return ":You are now an IRC operator"; }
	string RPL_REHASHING(string configFile) { return configFile + " :Rehashing"; } // Won't use
	string RPL_YOURESERVICE(string service) { return "You are service " + service; }
	string RPL_TIME(Server& server) { return server.name() + " :" + getDateTime(); }
	string RPL_USERSSTART(void) { return ":UserID   Terminal  Host"; }
	string RPL_USERS(string username, string ttyline, string hostname) { return ":" + username + " " + ttyline + " " + hostname; }
	string RPL_ENDOFUSERS(void) { return ":End of users"; }
	string RPL_NOUSERS(void) { return ":Nobody logged in"; }

	// string RPL_TRACELINK(string versionAndDebug, string destination, string nextServer, string protoVersion, string uptime, string backQueue, string upQueue) { return "Link " + versionAndDebug + " " + destination + " " + nextServer + " V" + protoVersion + " " + uptime + " " + backQueue + " " + upQueue; } // Will not use
	// string RPL_TRACECONNECTING(string klass, string server) { return "Try. " + klass + " " + server; }
	// string RPL_TRACEHANDSHAKE(string klass, string server) { return "H.S. " + klass + " " + server; }
	// string RPL_TRACEUNKNOWN(string klass, string ip) { return "???? " + klass + " " + ip; }
	// string RPL_TRACEOPERATOR(string klass, string nickname) { return "Oper " + klass + " " + nickname; }
	// string RPL_TRACEUSER(string klass, string nickname) { return "User " + klass + " " + nickname; }
	// // string RPL_TRACESERVER() { return ""; }
	// string RPL_TRACESERVICE(string klass, string name, string type, string activeType) { return "Service " + klass + " " + name + " " + type + " " + activeType; }
	// string RPL_TRACENEWTYPE(string newType, string clientName) { return newType + " 0 " + clientName; }
	// string RPL_TRACECLASS(string klass, string count) { return "Class " + klass + " " + count; }
	// // string RPL_TRACERECONNECT() { return ""; }
	// string RPL_TRACELOG(string logFile, string debugLevel) { return "File " + logFile + " " + debugLevel; }
	// string RPL_TRACEEND(string serverName, string versionAndDebug) { return serverName + " " + versionAndDebug + " :End of TRACE"; }

	// string RPL_STATSLINKINFO(string linkName, string sendQ, string messagesSent, string kbytesSent, string messagesReceived, string kbytesReceived, string timeOpen) { return linkName + " " + sendQ + " " + messagesSent + " " + kbytesSent + " " + messagesReceived + " " + kbytesReceived + " " + timeOpen; }
	// string RPL_STATSCOMMANDS(string command, string count, string bytesCount, string remoteCount) { return command + " " + count + " " + bytesCount + " " + remoteCount; }
	// string RPL_ENDOFSTATS(string statsLetter) { return statsLetter + " :End of STATS report"; }
	// string RPL_STATSUPTIME(string uptime) { return ":Server Up " + uptime; }
	// string RPL_STATSOLINE(string hostMask, string name) { return "O " + hostMask + " * " + name; }
	string RPL_UMODEIS(UserIRC* user) { return user->getModes(); }
	// string RPL_SERVLIST() { return ""; }
	// string RPL_SERVLISTEND() { return ""; }

	string RPL_LUSERCLIENT(string userCount, string serviceCount, string serverCount) { return ":There are " + userCount + " users and " + serviceCount + " services on " + serverCount + "servers"; }
	string RPL_LUSEROP(string operCount) { return operCount + " :operators(s) online"; }
	string RPL_LUSERUNKNOWN(string count) { return count + " :unknown connection(s)"; }
	string RPL_LUSERCHANNELS(string channelCount) { return channelCount + " :channels formed"; }
	string RPL_LUSERME(string userCount, string serverCount) { return ":I have " + userCount + " clients and " + serverCount + " servers"; }
	string RPL_ADMINME(string server) { return server + " :Administrative info"; }
	string RPL_ADMINLOC1(string adminInfo) { return ":" + adminInfo; }
	string RPL_ADMINLOC2(string adminInfo) { return ":" + adminInfo; }
	string RPL_ADMINEMAIL(string adminInfo) { return ":" + adminInfo; }
	string RPL_TRYAGAIN(string command) { return command + " :Please wait a while and try again"; }

	// ---- Errors
	string ERR_NOSUCHNICK(string nickname) { return nickname + " :No such nick/channel"; }
	string ERR_NOSUCHSERVER(string server) { return server + " :No such server"; }
	string ERR_NOSUCHCHANNEL(void) { return "No such channel"; }
	string ERR_CANNOTSENDTOCHAN(string channel) { return channel + " :Cannot send to channel"; }
	string ERR_TOOMANYCHANNELS(string channel) { return channel + " :You have joined too many channels"; }

	string ERR_WASNOSUCHNICK(string nickname) { return nickname + " :There was no such nickname"; }
	string ERR_TOOMANYTARGETS(string target, string errorCode, string abortMessage) { return target + " :" + errorCode + " recipients. " + abortMessage; }
	string ERR_NOSUCHSERVICE(string service) { return service + " :No such service"; }
	string ERR_NOORIGIN(void) { return ":No origin specified"; }
	string ERR_NORECIPIENT(string command) { return ":No recipient given (" + command + ")"; }
	string ERR_NOTEXTTOSEND(void) { return ":No text to send"; }
	string ERR_NOTOPLEVEL(string mask) { return mask + " :No toplevel domain specified"; }
	string ERR_WILDTOPLEVEL(string mask) { return mask + ":Wildcard in toplevel domain"; }
	string ERR_BADMASK(string mask) { return mask + ":Bad Server/host mask"; }

	string ERR_UNKNOWNCOMMAND(string command) { return command + " :Unknown command"; }
	string ERR_NOMOTD(void) { return "MOTD File is missing"; }
	string ERR_NOADMININFO(string server) { return server + " :No administrative info available"; }
	string ERR_FILEERROR(string fileOp, string file) { return ":File error doing " + fileOp + " on " + file; }
	string ERR_NONICKNAMEGIVEN(void) { return ":No nickname given"; }
	string ERR_ERRONEUSNICKNAME(string nickname) { return nickname + " :Erroneous nickname"; }
	string ERR_NICKNAMEINUSE(string nickname) { return nickname + " :Nickname is already in use"; }
	string ERR_NICKCOLLISION(string nickname, string user, string host) { return nickname + " :Nickname collision KILL from " + user + "@" + host; }

	string ERR_UNAVAILRESOURCE(string ressource) { return ressource + " :Nick/channel is temporarily unavailable"; }
	string ERR_USERNOTINCHANNEL(void) { return "They aren't on that channel"; }
	string ERR_NOTONCHANNEL(string channel) { return channel + " :You're not on that channel"; }
	string ERR_USERONCHANNEL(string nickname, string channel) { return nickname + " " + channel + " :is already on channel"; }
	string ERR_NOLOGIN(string user) { return user + " :user not logged in"; }
	string ERR_SUMMONDISABLED(void) { return ":SUMMON has been disabled"; }
	string ERR_USERSDISABLED(void) { return ":USERS has been disabled"; }
	string ERR_NOTREGISTERED(void) { return ":You have not registered"; }
	string ERR_NEEDMOREPARAMS(void) { return "Not enough parameters"; }
	string ERR_ALREADYREGISTRED(void) { return ":Unauthorized command (already registered)"; }
	string ERR_NOPERMFORHOST(void) { return ":Your host isn't among the privileged"; }
	string ERR_PASSWDMISMATCH(void) { return ":Password incorrect"; }
	string ERR_YOUREBANNEDCREEP(void) { return ":You are banned from this server"; }
	string ERR_YOUWILLBEBANNED(void) { return ""; }

	string ERR_KEYSET(string channel) { return channel + " :Channel key already set"; }
	string ERR_CHANNELISFULL(string channel) { return channel + " :Cannot join channel (+1)"; }
	string ERR_UNKNOWNMODE(string chara, string channel) { return chara + " :is unknown mode char to me for " + channel; }
	string ERR_INVITEONLYCHAN(string channel) { return channel + " :Cannot join channel (+i)"; }
	string ERR_BANNEDFROMCHAN(string channel) { return channel + " :Cannot join channel (+b)"; }
	string ERR_BADCHANNELKEY(string channel) { return channel + " :Cannot join channel (+k)"; }
	string ERR_BADCHANMASK(string channel) { return channel + " :Bad Channel Mask"; }
	string ERR_NOCHANMODES(string channel) { return channel + " :Channel doesn't support modes"; }
	string ERR_BANLISTFULL(string channel, string chara) { return channel + " " + chara + " :Channel list is full"; }

	string ERR_NOPRIVILEGES(void) { return ":Permission Denied- You're not an IRC operator"; }
	string ERR_CHANOPRIVSNEEDED(string channel) { return channel + " :You're not channel operator"; }
	string ERR_CANTKILLSERVER(void) { return ":You can't kill a server!"; }
	string ERR_RESTRICTED(void) { return ":Your connection is restricted!"; }
	string ERR_UNIQOPPRIVSNEEDED(void) { return ":You're not the original channel operator"; }
	string ERR_NOOPERHOST(void) { return ":No O-lines for your host"; }
	string ERR_UMODEUNKNOWNFLAG(void) { return "Unknown MODE flag"; }
	string ERR_USERSDONTMATCH(void) { return ":Cannot change mode for other users"; }


	string toString(int code) {
		stringstream sstream;

		sstream << code;
		string codeString = sstream.str();

		return string(3 - codeString.length(), '0').append(codeString);
	};
}


#endif
