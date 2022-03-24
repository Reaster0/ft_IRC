#ifndef _CHANNEL_
#define _CHANNEL_
#include <iostream>
#include <vector>
#include <map>
#include "UserIRC.hpp"
#include "MsgIRC.hpp"
#include "ServerClass.hpp"

namespace MODES {
	namespace CHANNEL {
		static const char ANONYMOUS				= 'a';
		static const char INVITE_ONLY			= 'i';
		static const char MODERATED				= 'm';
		static const char NO_OUTSIDE_MESSAGES	= 'n';
		static const char QUIET					= 'q';
		static const char PRIVATE				= 'p';
		static const char SECRET				= 's';
		static const char REOP					= 'r';
		static const char SETTABLE_TOPIC		= 't';
		static const char USER_LIMIT_SET		= 'l';

		static const char CREATOR				= 'O';
		static const char OPERATOR				= 'o';
		static const char VOICE					= 'v';

		static const string ALL					= "aimnqpsrtl";

		static const string USER_RELATED		= "Oov";
		static const string NEED_PARAMS			= "lOov";
		static const string TOGGLEABLE			= "aimnqpsrtl";
	}
}


class Server;
class ChannelModes {
public:
	ChannelModes() : anonymous(false)
	, inviteOnly(false)
	, moderated(false)
	, noOutsideMessages(false)
	, quiet(false)
	, privateChannel(false)
	, secret(false)
	, reop(false)
	, topicSettable(false)
	, keySet(false)
	, limitSet(false)
	, banMaskSet(false)
	, exceptionMaskSet(false)
	, invitationMaskSet(false) {}

	bool anonymous;
	bool inviteOnly;
	bool moderated;
	bool noOutsideMessages;
	bool quiet;
	bool privateChannel;
	bool secret;
	bool reop;
	bool topicSettable;
	bool keySet;
	bool limitSet;
	bool banMaskSet;
	bool exceptionMaskSet;
	bool invitationMaskSet;

	static bool is(char mode, const string& modes) {
		for (string::const_iterator it = modes.begin(); it != modes.end(); it++) {
			if (*it == mode) { return true; }
		}
		return false;
	};

	static bool exist(char mode) {
		for (string::const_iterator it = MODES::CHANNEL::ALL.begin(); it != MODES::CHANNEL::ALL.end(); it++) {
			if (*it == mode) { return true; }
		}

		for (string::const_iterator it = MODES::CHANNEL::USER_RELATED.begin(); it != MODES::CHANNEL::USER_RELATED.end(); it++) {
			if (*it == mode) { return true; }
		}
		return false;
	};

	class UnknownMode : public exception {
		const char* what(void) const throw();
	};
};

class UserChannelModes {
public:
	UserChannelModes() : creator(false)
	, channelOperator(false)
	, voice(false) {}

	bool creator;
	bool channelOperator;
	bool voice;

	class UnknownMode : public exception {
		const char* what(void) const throw();
	};

	class UnknownUser : public exception {
		const char* what(void) const throw();
	};
};


class Channel
{
	public:
		int			_maximum_users;
		std::string	_topic;
		std::string	_name;

		map<UserIRC*, UserChannelModes> user_modes;

		vector<UserIRC*> current_users;
		vector<UserIRC*> banned_users;
		vector<UserIRC*> invited_users;

		ChannelModes modes;

		Channel();
		Channel(string name);
		Channel(const Channel &chan);
		Channel& operator=(const Channel &rhs);
		~Channel();

		string getModes(void) const;
		bool getMode(char mode) const;
		void setMode(char mode, bool value);
		string getUserModes(UserIRC* user) const;
		bool getUserMode(UserIRC* user, char mode) const;
		void setUserMode(UserIRC* user, char mode, bool value);

		bool	isAuthorizedUser(UserIRC *user);
		void	acceptUser(UserIRC *user);
		void	getInfo(void);
		void	sendToAll(PayloadIRC& payload, Server& server, UserIRC* exception = 0);
		bool	isInChannel(UserIRC *user);
		bool	isInvited(UserIRC *user);

		//return a string of all users in the chan separated by space
		string	userList();
		//is protected against user that dosen't are in the chan
		void	removeUsersFromChan(UserIRC *user);
		
	private:
		//return current_users.end() if not found
		vector<UserIRC*>::iterator UserIterator(UserIRC *user);

};

#endif
