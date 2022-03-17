#ifndef _USERIRC_
#define _USERIRC_
#include <algorithm>
#include <list>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <queue>
#include <map>

using namespace std;

namespace MODES {
	static const char AWAY				= 'a';
	static const char INVISIBLE			= 'i';
	static const char WALLOPS			= 'w';
	static const char RESTRICTED		= 'r';
	static const char OPERATOR			= 'o';
	static const char LOCAL_OPERATOR	= 'O';
	static const char SERVER_NOTICES	= 's';

	static const string ALL				= "aiwroOs";
}

class UserModes {
public:
	UserModes()
	: away(false)
	, invisible(false)
	, wallops(false)
	, restricted(false)
	, isOperator(false)
	, isLocalOperator(false)
	, serverNotices(false)
	{}

	bool away;
	bool invisible;
	bool wallops;
	bool restricted;
	bool isOperator;
	bool isLocalOperator;
	bool serverNotices;

	static bool exist(char mode) {
		for (string::const_iterator it = MODES::ALL.begin(); it != MODES::ALL.end(); it++) {
			if (*it == mode) { return true; }
		}
		return false;
	};

	class UnknownMode : public exception {
		const char* what(void) const throw();
	};
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
	string ip;
	time_t idle;
	bool	allowed;

	bool away;
	string awayMessage;
	UserModes modes;

	string getModes(void) const;
	bool getMode(char mode) const;
	void setMode(char mode, bool value);
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
