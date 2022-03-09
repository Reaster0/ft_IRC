#ifndef _CHANNEL_
#define _CHANNEL_
#include <iostream>
#include <vector>
#include "UserIRC.hpp"
#include "MsgIRC.hpp"
#include "ServerClass.hpp"

class Server;
class Channel
{
	public:
		bool		_private;
		bool		_secrete;
		bool		_limited_capacity;
		int			_maximum_users;
		std::string	_topic;
		std::string	_name;

		vector<UserIRC*> current_users;
		vector<UserIRC*> banned_users;
		vector<UserIRC*> invited_users;

		Channel();
		Channel(string name);
		~Channel();

		bool	isAuthorizedUser(UserIRC *user);
		void	acceptUser(UserIRC *user);
		void	getInfo(void);
		void	sendToAll(PayloadIRC& payload, Server& server);

};

#endif
