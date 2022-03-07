#include <string>
#include <string.h>
#include <poll.h>
#include <iostream>
#include <vector>
#include <map>
#include "server.hpp"
#include "Channel.hpp"

std::string g_pwd;

int main()
{
	g_pwd = randomPwd(10);
	std::cout << "password:\n	";
	std::cout << g_pwd <<std::endl;
	int endpoint = createServer(6667);
	serverLoop(endpoint);
}

/*
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
	std::cout << "[DEBUG MODE]" << std::endl;
    int client, server;
    int portNum = 1600;
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

	chan.banned_users.push_back(&user);
	chan.acceptUser(&user);
	chan.getInfo();

    while (server > 0) 
    {
        send(server, buffer, bufsize, 0);
        cout << "Client: ";
		recv(server, buffer, bufsize, 0);
		cout << buffer << std::endl;
    }
	close(server);
    close(client);
    return 0;
}
*/