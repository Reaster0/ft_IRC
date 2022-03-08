#ifndef _USERIRC_
#define _USERIRC_
#include <algorithm>
#include <list>
#include "ServerClass.hpp"

using namespace std;

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
	list<UserIRC> listC;

	public:
	UserList(){}
	~UserList(){}
	
	void addUser(UserIRC& user);
	int acceptNew();
	UserIRC* findByUsername(const string& value);
	UserIRC* findBySocket(const int& value);
};

#endif
