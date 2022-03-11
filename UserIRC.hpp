#ifndef _USERIRC_
#define _USERIRC_
#include <algorithm>
#include <list>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <queue>

using namespace std;

struct UserModes {
	bool away;
	bool invisible;
	bool wallops;
	bool restricted;
	bool isOperator;
	bool isLocalOperator;
	bool serverNotices;
};

struct UserIRC
{
	int fdSocket;
	string username;
	string nickname;
	string realName;
	sockaddr_in addr;
	socklen_t sockLen;
	bool needFill;

	bool away;
	string awayMessage;
	UserModes modes;
};


class UserList
{
	private:
	list<UserIRC> listC;

	public:
	UserList(){}
	~UserList(){}
	
	void addUser(UserIRC& user);
	void removeUser(int& fd);
	UserIRC* acceptNew(const int& endpoint);
	UserIRC* findByUsername(const string& value);
	UserIRC* findByNickname(const string& value);
	UserIRC* findBySocket(const int& value);
	UserIRC* findFirstUnfilled();
};

string getIPAddress(const UserIRC* user);


#endif
