#ifndef _USERIRC_
#define _USERIRC_
#include "server.hpp"
#include <map>

struct UserIRC
{
	int fdSocket;
	string username;
	string nickname;
	sockaddr_in addr;
	socklen_t sockLen;
};


class UserList
{
	private:
	map<string, UserIRC> list;

	public:
	UserList(){}
	~UserList(){}
	
	void addUser(UserIRC& user);
	int acceptNew();
	UserIRC& operator[](const string& value);
};

#endif