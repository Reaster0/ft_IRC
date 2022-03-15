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

struct UserIRC
{
	int fdSocket;
	string username;
	string nickname;
	string realName;
	sockaddr_in addr;
	socklen_t sockLen;
	bool needFill;
	string ip;
	time_t idle;

	bool away;
	string awayMessage;
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
	size_t size() const;
	//UserIRC* findFirstUnfilled(); depreciated
};

string getIPAddress(const UserIRC* user);


#endif
