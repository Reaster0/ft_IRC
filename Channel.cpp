#include "Channel.hpp"
#include <iomanip>

Channel::Channel(std::string name)
{
	_name = name;
	_maximum_users = 0;
	_topic = "undefined_topic";
	setMode(MODES::CHANNEL::NO_OUTSIDE_MESSAGES, true);
}

Channel::Channel()
{
	_name = "undefined_name";
	_maximum_users = 0;
	_topic = "undefined_topic";
	setMode(MODES::CHANNEL::NO_OUTSIDE_MESSAGES, true);
}

Channel::~Channel(){}

// return false if the user in argument has been kicked from chan
bool	Channel::isAuthorizedUser(UserIRC *user)
{
	if (std::find(banned_users.begin(), banned_users.end(), user) != banned_users.end())
		return false;
	return true;
}

Channel::Channel(const Channel &chan)
{
	*this = chan;
}

Channel& Channel::operator=(const Channel &rhs)
{
	if (this != &rhs)
	{
		_name = rhs._name;
		_maximum_users = rhs._maximum_users;
		_topic = rhs._topic;
		current_users = rhs.current_users;
		invited_users = rhs.invited_users;
		banned_users = rhs.banned_users;
		modes = rhs.modes;
		user_modes = rhs.user_modes;
	}
	return *this;
}

string Channel::getModes(void) const {
	string modes;

	for (string::const_iterator it = MODES::CHANNEL::ALL.begin(); it != MODES::CHANNEL::ALL.end(); it++) {
		if (this->getMode(*it)) { modes += *it; }
	}
	if (modes.size() > 0)
		return "+" + modes;
	return "";
}

bool Channel::getMode(char mode) const {
	switch (mode) {
		case MODES::CHANNEL::ANONYMOUS:
			return this->modes.anonymous;
		case MODES::CHANNEL::INVITE_ONLY:
			return this->modes.inviteOnly;
		case MODES::CHANNEL::MODERATED:
			return this->modes.moderated;
		case MODES::CHANNEL::NO_OUTSIDE_MESSAGES:
			return this->modes.noOutsideMessages;
		case MODES::CHANNEL::QUIET:
			return this->modes.quiet;
		case MODES::CHANNEL::PRIVATE:
			return this->modes.privateChannel;
		case MODES::CHANNEL::SECRET:
			return this->modes.secret;
		case MODES::CHANNEL::REOP:
			return this->modes.reop;
		case MODES::CHANNEL::SETTABLE_TOPIC:
			return this->modes.topicSettable;
		case MODES::CHANNEL::USER_LIMIT_SET:
			return this->modes.limitSet;
		default:
			throw ChannelModes::UnknownMode();
	}
}

void Channel::setMode(char mode, bool value) {
	switch (mode) {
		case MODES::CHANNEL::ANONYMOUS:
			this->modes.anonymous = value;
			break;
		case MODES::CHANNEL::INVITE_ONLY:
			this->modes.inviteOnly = value;
			break;
		case MODES::CHANNEL::MODERATED:
			this->modes.moderated = value;
			break;
		case MODES::CHANNEL::NO_OUTSIDE_MESSAGES:
			this->modes.noOutsideMessages = value;
			break;
		case MODES::CHANNEL::QUIET:
			this->modes.quiet = value;
			break;
		case MODES::CHANNEL::PRIVATE:
			this->modes.privateChannel = value;
			break;
		case MODES::CHANNEL::SECRET:
			this->modes.secret = value;
			break;
		case MODES::CHANNEL::REOP:
			this->modes.reop = value;
			break;
		case MODES::CHANNEL::SETTABLE_TOPIC:
			this->modes.topicSettable = value;
			break;
		case MODES::CHANNEL::USER_LIMIT_SET:
			this->modes.limitSet = value;
			break;
		default:
			throw ChannelModes::UnknownMode();
	}
}

const char* ChannelModes::UnknownMode::what(void) const throw() {
	return "Unknown mode";
}

const char* UserChannelModes::UnknownMode::what(void) const throw() {
	return "Unknown mode";
}

const char* UserChannelModes::UnknownUser::what(void) const throw() {
	return "Unknown user";
}

bool Channel::getUserMode(UserIRC* user, char mode) const {
	map<UserIRC*, UserChannelModes>::const_iterator itNode = this->user_modes.find(user);

	if (itNode == this->user_modes.end()) {
		throw UserChannelModes::UnknownUser();
	}

	switch (mode) {
		case MODES::CHANNEL::CREATOR:
			return itNode->second.creator;
		case MODES::CHANNEL::OPERATOR:
			return itNode->second.channelOperator;
		case MODES::CHANNEL::VOICE:
			return itNode->second.voice;
		default:
			throw UserChannelModes::UnknownMode();
	}
}

void Channel::setUserMode(UserIRC* user, char mode, bool value) {
	map<UserIRC*, UserChannelModes>::iterator itNode = this->user_modes.find(user);

	if (itNode == this->user_modes.end()) {
		throw UserChannelModes::UnknownUser();
	}

	switch (mode) {
		case MODES::CHANNEL::CREATOR:
			itNode->second.creator = value;
			break;
		case MODES::CHANNEL::OPERATOR:
			itNode->second.channelOperator = value;
			break;
		case MODES::CHANNEL::VOICE:
			itNode->second.voice = value;
			break;
		default:
			throw UserChannelModes::UnknownMode();
	}
}

string Channel::getUserModes(UserIRC* user) const {
	string modes;

	for (string::const_iterator it = MODES::CHANNEL::USER_RELATED.begin(); it != MODES::CHANNEL::USER_RELATED.end(); it++) {
		if (this->getUserMode(user, *it)) { modes += *it; }
	}
	if (modes.size() > 0)
		return "+" + modes;
	return "";
}
// include authorized user inside chan's current_users and remove from chan's invited list
void	Channel::acceptUser(UserIRC *user)
{
	UserChannelModes modes;
	for(std::vector<UserIRC*>::iterator iter2 = current_users.begin(); iter2 != current_users.end(); ++iter2)
	{
		if(*iter2 == user)
			return;
	}
	if (isAuthorizedUser(user) == false)
	{
		// cout << user->username << "(" << getIPAddress(user) <<  ")";
		// cout << " cannot connect to " << _name << endl;
		return;
	}
	for(std::vector<UserIRC*>::iterator iter = invited_users.begin(); iter != invited_users.end(); ++iter)
	{
		if(*iter == user)
		{
			invited_users.erase( iter );
			break;
		}
	}
	current_users.push_back(user);
	user_modes.insert(pair<UserIRC*, UserChannelModes>(user, modes));
	if (current_users.size() == 1) {
		setUserMode(user, MODES::CHANNEL::CREATOR, true);
	}

	// cout << user->username << "(" << getIPAddress(user) <<  ")";
	// cout << " is connected to " << _name << endl;
}

void	Channel::getInfo(void)
{
	std::cout << "===========================================" << std::endl;
	std::cout << "CHANNEL" << std::endl;
	std::cout << "_name:    " <<  setw(20) << right  << _name  << std::endl;
	std::cout << "_private: " << setw(20) << right  << (getMode(MODES::CHANNEL::PRIVATE) ? "true" : "false") << std::endl; 
	std::cout << "_secrete: " << setw(20) << right  << (getMode(MODES::CHANNEL::SECRET) ? "true" : "false") << std::endl; 
	std::cout << "_limited_capacity:  " << setw(10) << right  << (getMode(MODES::CHANNEL::USER_LIMIT_SET) ? "true" : "false") << std::endl; 
	std::cout << "_maximum_users:" << setw(15) << right  << _maximum_users << std::endl;
	std::cout << "_topic:   "  <<  setw(20) << right  << _topic << std::endl;

	std::cout << "\nCURENT_USERS" << std::endl;
	for(std::vector<UserIRC*>::iterator iter = current_users.begin(); iter != current_users.end(); ++iter)
		std::cout  << (*iter)->nickname << std::setw(20) << (*iter)->username  << std::setw(10) << getIPAddress((*iter)) << std::endl;

	std::cout << "\nBANNED_USERS" << std::endl;
	for(std::vector<UserIRC*>::iterator iter = banned_users.begin(); iter != banned_users.end(); ++iter)
		std::cout   << (*iter)->nickname << std::setw(20) << (*iter)->username << std::setw(10) << getIPAddress((*iter)) << std::endl;

	std::cout << "\nINVITED_USERS" << std::endl;
	for(std::vector<UserIRC*>::iterator iter = invited_users.begin(); iter != invited_users.end(); ++iter)
		std::cout  << (*iter)->nickname << std::setw(20) << (*iter)->username << std::setw(10) << getIPAddress((*iter)) << std::endl;

	std::cout << "===========================================" << std::endl;
}

void    Channel::sendToAll(PayloadIRC& payload, Server &server, UserIRC* exception)
{

    for(std::vector<UserIRC*>::iterator iter = current_users.begin(); iter != current_users.end(); ++iter)
    {
		if (exception && *iter == exception)
			continue;
		cout << "sending payload:" << payload.command << " to user:" << (*iter)->nickname << std::endl;
        MsgIRC msg(*iter, payload);
        server._msgQueue.push(msg);
    }
}

bool	Channel::isInChannel(UserIRC *user)
{
	for(std::vector<UserIRC*>::iterator iter = current_users.begin(); iter != current_users.end(); ++iter)
    {
        if ((*iter) == user)
			return true;
    }
	return false;
}

bool	Channel::isInvited(UserIRC *user)
{
	for(std::vector<UserIRC*>::iterator iter = invited_users.begin(); iter != invited_users.end(); ++iter)
    {
        if ((*iter) == user)
			return true;
    }
	return false;
}

vector<UserIRC*>::iterator Channel::UserIterator(UserIRC *user)
{
	for(std::vector<UserIRC*>::iterator iter = current_users.begin(); iter != current_users.end(); ++iter)
    {
        if ((*iter) == user)
			return iter;
    }
	return current_users.end();
}

void	Channel::removeUsersFromChan(UserIRC *user)
{
	for(std::vector<UserIRC*>::iterator iter = current_users.begin(); iter != current_users.end(); ++iter)
    {
        if ((*iter) == user)
		{
			current_users.erase(iter);
			user_modes.erase(user_modes.find(user));
			return;
		}
    }
}

string	Channel::userList()
{
	string result = "";
	for (std::vector<UserIRC*>::iterator iter = current_users.begin(); iter != current_users.end(); ++iter)
	{
		if (result != "")
			result += " ";
		result += (*iter)->nickname;
	}
	return result;
}
