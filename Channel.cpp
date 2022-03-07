#include "Channel.hpp"
#include <iomanip>

Channel::Channel(std::string name)
{
	_name = name;
	_private = false;
	_secrete = false;
	_limited_capacity = false;
	_maximum_users = 0;
	_topic = "undefined_topic";
}

Channel::~Channel(){}

// return false if the user in argument has been kicked from chan
bool	Channel::isAuthorizedUser(UserIRC *user)
{
	if (std::find(banned_users.begin(), banned_users.end(), user) != banned_users.end())
		return false;
	return true;
}

// include authorized user inside chan's current_users and remove from chan's invited list
void	Channel::acceptUser(UserIRC *user)
{
	for(std::vector<UserIRC*>::iterator iter2 = current_users.begin(); iter2 != current_users.end(); ++iter2)
	{
		if(*iter2 == user)
			return;
	}
	if (isAuthorizedUser(user) == false)
	{
		cout << user->username << "(" << getIPAddress(user->fdSocket) <<  ")";
		cout << " cannot connect to " << _name << endl;
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
	cout << user->username << "(" << getIPAddress(user->fdSocket) <<  ")";
	cout << " is connected to " << _name << endl;
}

void	Channel::getInfo(void)
{
	std::cout << "===========================================" << std::endl;
	std::cout << "CHANNEL" << std::endl;
	std::cout << "_name:    " <<  setw(20) << right  << _name  << std::endl;
	std::cout << "_private: " << setw(20) << right  << (_private ? "true" : "false") << std::endl; 
	std::cout << "_secrete: " << setw(20) << right  << (_secrete ? "true" : "false") << std::endl; 
	std::cout << "_limited_capacity:  " << setw(10) << right  << (_limited_capacity ? "true" : "false") << std::endl; 
	std::cout << "_maximum_users:" << setw(15) << right  << _maximum_users << std::endl;
	std::cout << "_topic:   "  <<  setw(20) << right  << _topic << std::endl;

	std::cout << "\nCURENT_USERS" << std::endl;
	for(std::vector<UserIRC*>::iterator iter = current_users.begin(); iter != current_users.end(); ++iter)
		std::cout  << (*iter)->nickname << std::setw(20) << (*iter)->username  << std::setw(10) << getIPAddress((*iter)->fdSocket) << std::endl;

	std::cout << "\nBANNED_USERS" << std::endl;
	for(std::vector<UserIRC*>::iterator iter = banned_users.begin(); iter != banned_users.end(); ++iter)
		std::cout   << (*iter)->nickname << std::setw(20) << (*iter)->username << std::setw(10) << getIPAddress((*iter)->fdSocket) << std::endl;

	std::cout << "\nINVITED_USERS" << std::endl;
	for(std::vector<UserIRC*>::iterator iter = invited_users.begin(); iter != invited_users.end(); ++iter)
		std::cout  << (*iter)->nickname << std::setw(20) << (*iter)->username << std::setw(10) << getIPAddress((*iter)->fdSocket) << std::endl;

	// int x = 0;
	//  while (x < 10)
	//  {
    //     cout <<  x << setw(10) << right << setw(10) << (x*x) << endl;
	// 	x++;
	//  }
	std::cout << "===========================================" << std::endl;
}