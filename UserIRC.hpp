#ifndef _USERIRC_
#define _USERIRC_
#include "server.hpp"
#include "MsgIRC.hpp"
#include <algorithm>
#include <list>

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
	int acceptNew(const int& endpoint);
	UserIRC* findByUsername(const string& value);
	UserIRC* findBySocket(const int& value);
	UserIRC* findFirstUnfilled();
};

#endif