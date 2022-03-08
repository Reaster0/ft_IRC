#ifndef _MESSAGESIRC_
#define _MESSAGESIRC_
#include "UserIRC.hpp"
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <sstream>

using namespace std;

struct UserIRC;

struct PayloadIRC
{
	string command;
	list<string> params;
	string prefix;
	string trailer;
	
	PayloadIRC(){}
	PayloadIRC(const PayloadIRC& other):
	command(other.command), params(other.params),
	prefix(other.prefix), trailer(other.trailer){}
	bool empty();
};

struct MsgIRC
{
	UserIRC* sender;
	UserIRC* receiver;
	PayloadIRC payload;

	MsgIRC() : sender(0), receiver(0){}
	MsgIRC(UserIRC* _receiver, const PayloadIRC& _payload):
	sender(0), receiver(_receiver), payload(_payload){}
	MsgIRC(UserIRC* _sender,UserIRC* _receiver, const PayloadIRC& _payload):
	sender(_sender), receiver(_receiver), payload(_payload){}
	~MsgIRC(){}
	//add the function to call payload later
};

//send the message and remove it from the writelist (return send)
size_t sendMsg(fd_set &availableWSockets, MsgIRC& msg);

//read and parse the msg 
size_t receiveMsg(const int& socket, fd_set &availableRSockets, MsgIRC& msg);

void parsingToPayload(char* buffer, PayloadIRC& payload);

#endif
