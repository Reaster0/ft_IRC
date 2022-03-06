#include "UserIRC.hpp"

void UserList::addUser(UserIRC& user)
{
	list[user.username] = user;
}

int UserList::acceptNew()
{
	return 1; //wip
}

UserIRC& UserList::operator[](const string& value)
{
	return list[value];
}