#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>
#include "ServerClass.hpp"
#include "Channel.hpp"

std::string g_pwd;

// int main()
// {
// 	g_pwd = randomPwd(10);
// 	std::cout << "password:\n	";
// 	std::cout << g_pwd << std::endl;
// 	// int endpoint = create_server(6667);
// 	// server_loop(endpoint);
//     Server server;
//     server.launch();
// }

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "Channel.hpp"
#include "UserIRC.hpp"

using namespace std;

int main()
{
	std::cout << "[DEBUG MODE]" << std::endl;
    int client, server;
    int portNum = 20000;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
	bzero(buffer, bufsize);

    struct sockaddr_in server_addr;
    socklen_t size;
    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }
    cout << "=> Socket server has been created..." << endl;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);
    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }
    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;
    listen(client, 1);
    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);
    if (server < 0) 
        cout << "=> Error on accepting..." << endl;
	cout << "=> Connection with IP " << inet_ntoa(server_addr.sin_addr) << std::endl;

	cout << "creation of user..." << std::endl;
	UserIRC user;
	user.nickname = "polo";
	user.username = "polo_username59";
	user.fdSocket = client;
	
	cout << "creation of chan...\n\n" << std::endl;
	Channel chan("random_chan");

	// chan.banned_users.push_back(&user);
	chan.acceptUser(&user);
	chan.getInfo();

	MsgIRC msg;
	MsgIRC response;
	Server server;

    while (server > 0) 
    {     
		bzero(buffer, bufsize);   
		recv(server, buffer, bufsize, 0);
		std::cout << "BUFFER =>[" << buffer << "]" <<std::endl;
		parsingToPayload(buffer, msg.payload);
		printPayload(msg.payload);

		std::cout << "CMD =>[" << msg.payload.command << "]" <<std::endl;
		if (msg.payload.command == "/JOIN")
		{
			if (msg.payload.params.empty())
				break;
			server._channels.insert( std::pair<string, Channel>( msg.payload.params.front(), Channel(msg.payload.params.front() ) ) );		
			std::cout << "chan => " << msg.payload.params.front() << std::endl;
		}
		msg = MsgIRC();
		std::cout << std::endl;
    }
	close(server);
    close(client);
    return 0;
}
