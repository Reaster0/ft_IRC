#ifndef _CHANNEL_
#define _CHANNEL_

#include <iostream>
#include <vector>
struct UserIRC;
#include "UserIRC.hpp"

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

		Channel(string name);
		~Channel();

		bool	isAuthorizedUser(UserIRC *user);
		void	acceptUser(UserIRC *user);
		void	getInfo(void);


};

#endif
