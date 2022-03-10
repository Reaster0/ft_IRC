#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>
#include "ServerClass.hpp"
#include "Channel.hpp"

int main()
{
	Server server;
    server.launch();
}

// #include <iostream>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include "Channel.hpp"
// #include "UserIRC.hpp"

// using namespace std;

// int main2()
// {
// 	std::cout << "[DEBUG MODE]" << std::endl;
//     int client, server;
//     int portNum = 20000;
//     bool isExit = false;
//     int bufsize = 1024;
//     char buffer[bufsize];
// 	bzero(buffer, bufsize);

//     struct sockaddr_in server_addr;
//     socklen_t size;
//     client = socket(AF_INET, SOCK_STREAM, 0);

//     if (client < 0) 
//     {
//         cout << "\nError establishing socket..." << endl;
//         exit(1);
//     }
//     cout << "=> Socket server has been created..." << endl;
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = htons(INADDR_ANY);
//     server_addr.sin_port = htons(portNum);
//     if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
//     {
//         cout << "=> Error binding connection, the socket has already been established..." << endl;
//         return -1;
//     }
//     size = sizeof(server_addr);
//     cout << "=> Looking for clients..." << endl;
//     listen(client, 1);
//     int clientCount = 1;
//     server = accept(client,(struct sockaddr *)&server_addr,&size);
//     if (server < 0) 
//         cout << "=> Error on accepting..." << endl;
// 	cout << "=> Connection with IP " << inet_ntoa(server_addr.sin_addr) << std::endl;

// 	cout << "creation of user..." << std::endl;
// 	UserIRC user;
// 	user.nickname = "polo";
// 	user.username = "polo_username59";
// 	user.fdSocket = server;
	
// 	cout << "creation of chan...\n\n" << std::endl;
// 	Channel chan("random_chan");

// 	// chan.banned_users.push_back(&user);
// 	// chan.acceptUser(&user);
// 	chan.getInfo();

// 	MsgIRC msg;
// 	MsgIRC response;
// 	Server server_class;

// 	msg.sender = &user; 

//     while (server > 0) 
//     {     
// 		bzero(buffer, bufsize);   
// 		recv(server, buffer, bufsize, 0);
// 		parsingToPayload(buffer, msg.payload);
// 		printPayload(msg.payload);

// 		std::cout << "CMD =>[" << msg.payload.command << "]" <<std::endl;
// 		if (msg.payload.command == "/JOIN")
// 		{
// 			if (msg.payload.params.empty())
// 				break;
// 			if (msg.payload.params.front()[0] != '#')
// 				msg.payload.params.front() = '#' + msg.payload.params.front();
// 			std::string chan_name = msg.payload.params.front();
// 			if (server_class._channels.find(chan_name) != server_class._channels.end() && server_class._channels[chan_name].isAuthorizedUser(msg.sender) == false)
// 				break;
// 			server_class._channels.insert( std::pair<string, Channel>(chan_name, Channel(chan_name)) );

// 			server_class._channels[chan_name].acceptUser(msg.sender);
// 			response.payload.prefix = msg.sender->nickname + "@" + getIPAddress(msg.sender);
// 			response.payload.command = "JOIN";
// 			response.payload.params.push_back(chan_name);

// 			//sending to all participants of the chan
// 			//server_class._channels[chan_name].sendToAll(response.payload, server);
// 				std::string temp =  msg.sender->nickname + "@" + getIPAddress(msg.sender) + " JOIN " + chan_name;
// 				std::cout << temp << std::endl;
// 				for(std::vector<UserIRC*>::iterator iter = server_class._channels[chan_name].current_users.begin(); iter != server_class._channels[chan_name].current_users.end(); ++iter)
// 					send((*iter)->fdSocket, &temp, sizeof(temp), 0);

// 		}
// 		msg = MsgIRC();
// 		std::cout << std::endl;
//     }
// 	close(server);
//     close(client);
//     return 0;
// }
