#ifndef _MESSAGESIRC_
#define _MESSAGESIRC_
#include "UserIRC.hpp"
#include <string>

using namespace std;

struct UserIRC;

struct MsgIRC
{
	UserIRC* sender;
	UserIRC* receiver;
	string payload;
	MsgIRC() : sender(0), receiver(0){}
	MsgIRC(UserIRC* _sender, UserIRC* _receiver, const string& _payload):
	sender(_sender), receiver(_receiver), payload(_payload){}
	~MsgIRC(){}
	//add the function to call payload later
};

//send the message and remove it from the writelist (return send)
size_t sendMsg(fd_set &availableWSockets, const MsgIRC& msg);

//read and parse the msg 
size_t receiveMsg(const int& socket, fd_set &availableRSockets, MsgIRC& msg);

#endif