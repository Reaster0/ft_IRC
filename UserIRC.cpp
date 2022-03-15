#include "UserIRC.hpp"

string UserIRC::getModes(void) const {
	string modes;

	for (string::const_iterator it = MODES::ALL.begin(); it != MODES::ALL.end(); it++) {
		if (this->getMode(*it)) { modes += *it; }
	}
	if (modes.size() > 0)
		return "+" + modes;
	return "";
}

bool UserIRC::getMode(char mode) const {
	switch (mode) {
		case MODES::AWAY:
			return this->modes.away;
		case MODES::INVISIBLE:
			return this->modes.invisible;
		case MODES::WALLOPS:
			return this->modes.wallops;
		case MODES::RESTRICTED:
			return this->modes.restricted;
		case MODES::OPERATOR:
			return this->modes.isOperator;
		case MODES::LOCAL_OPERATOR:
			return this->modes.isLocalOperator;
		case MODES::SERVER_NOTICES:
			return this->modes.serverNotices;
		default:
			throw UserModes::UnknownMode();
	}
}

void UserIRC::setMode(char mode, bool value) {
	switch (mode) {
		case MODES::AWAY:
			this->modes.away = value;
			break;
		case MODES::INVISIBLE:
			this->modes.invisible = value;
			break;
		case MODES::WALLOPS:
			this->modes.wallops = value;
			break;
		case MODES::RESTRICTED:
			this->modes.restricted = value;
			break;
		case MODES::OPERATOR:
			this->modes.isOperator = value;
			break;
		case MODES::LOCAL_OPERATOR:
			this->modes.isLocalOperator = value;
			break;
		case MODES::SERVER_NOTICES:
			this->modes.serverNotices = value;
			break;
		default:
			throw UserModes::UnknownMode();
	}
}

const char* UserModes::UnknownMode::what(void) const throw() {
	return "Unknown mode";
}


void UserList::addUser(UserIRC& user)
{
	listC.push_back(user);
}

void UserList::removeUser(int& fd)
{
	for (list<UserIRC>::iterator it = listC.begin(); it != listC.end(); ++it)
		if ((*it).fdSocket == fd)
		{
			listC.erase(it);
			return;
		}
}

//accept and fill a newuser that's added to the list (need to fill furthermore the client after)(return the socket)
UserIRC* UserList::acceptNew(const int& endpoint)
{
	UserIRC newOne;
	newOne.idle = time(0);
	newOne.fdSocket = accept(endpoint, (sockaddr*)&newOne.addr, &newOne.sockLen);
	newOne.ip = getIPAddress(&newOne);
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

UserIRC* UserList::findByNickname(const string& value)
{
	for (list<UserIRC>::iterator it = listC.begin(); it != listC.end(); ++it)
		if ((*it).nickname == value)
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

size_t UserList::size() const
{
	return listC.size();
}

// //return the first user that need filling or nothing
// UserIRC* UserList::findFirstUnfilled()
// {
// 	for (list<UserIRC>::iterator it = listC.begin(); it != listC.end(); ++it)
// 		if ((*it).needFill)
// 			return &(*it);
// 	return 0;
// }

string getIPAddress(const UserIRC* user)
{
	if (!user)
		return "";
	char buffer[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &user->addr.sin_addr.s_addr, buffer, INET_ADDRSTRLEN);
	return buffer;
}
