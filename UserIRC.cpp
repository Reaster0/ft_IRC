#include "UserIRC.hpp"

void UserList::addUser(UserIRC& user)
{
	listC.push_back(user);
}

int UserList::acceptNew()
{
	return 1; //wip
}

UserIRC* UserList::findByUsername(const string& value)
{
	for (list<UserIRC>::iterator it = listC.begin(); it != listC.end(); ++it)
		if ((*it).username == value)
			return &(*it);
	return 0;
}

UserIRC* UserList::findBySocket(const int& value)
{
	for (list<UserIRC>::iterator it = listC.begin(); it != listC.end(); ++it)
		if ((*it).fdSocket == value)
			return &(*it);
	return 0;
}