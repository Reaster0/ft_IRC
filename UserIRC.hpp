#ifndef _USERIRC_
#define _USERIRC_

#include "MsgIRC.hpp"
#include <algorithm>
#include <list>
#include "ServerClass.hpp"

using namespace std;

struct UserIRC
{
	int fdSocket;
	string username;
	string nickname;
	string realName;
	sockaddr_in addr;
	socklen_t sockLen;
	bool needFill;
};


class UserList
{
	private:
	list<UserIRC> listC;

	public:
	UserList(){}
	~UserList(){}
	
	void addUser(UserIRC& user);
	UserIRC* acceptNew(const int& endpoint);
	UserIRC* findByUsername(const string& value);
	UserIRC* findBySocket(const int& value);
	UserIRC* findFirstUnfilled();
};

#endif
