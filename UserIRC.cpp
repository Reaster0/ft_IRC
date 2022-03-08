#include "UserIRC.hpp"

void UserList::addUser(UserIRC& user)
{
	listC.push_back(user);
}

//accept and fill a newuser that's added to the list (need to fill furthermore the client after)(return the socket)
UserIRC* UserList::acceptNew(const int& endpoint)
{
	UserIRC newOne;
	newOne.fdSocket = accept(endpoint, (sockaddr*)&newOne.addr, &newOne.sockLen);
	newOne.needFill = true;
	listC.push_back(newOne);
	return &listC.back();
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

//return the first user that need filling or nothing
UserIRC* UserList::findFirstUnfilled()
{
	for (list<UserIRC>::iterator it = listC.begin(); it != listC.end(); ++it)
		if ((*it).needFill)
			return &(*it);
	return 0;
}
