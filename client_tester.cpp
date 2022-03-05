#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

bool g_exit = false;

void sighandler(int)
{ 
	g_exit = true;
}

int main()
{
    int client;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
	bzero(buffer, bufsize);
	string ip_str = "127.0.0.1";
    char* ip = const_cast<char*>(ip_str.c_str());
	string input;

    struct sockaddr_in server_addr;
	signal(SIGQUIT, sighandler);
	signal(SIGINT, sighandler);

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }
    cout << "\n=> Socket client has been created..." << endl;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;
    cout << "=> Awaiting confirmation from the server..." << endl;
   // recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed";
    cout << "\n\n=> Enter # to end the connection\n" << endl;

    while (g_exit == false)
	{
        cout << "Client: ";
		input.clear();
		getline(cin, input);
		if (input == "EXIT")
		{
			cout << "exit client"<< endl;
			input = "has quit";
			send(client, &input, sizeof(input), 0);
			break;
		}
		send(client, &input, sizeof(input), 0);
    } 
    cout << "\n=> Connection terminated.\nGoodbye...\n";
    close(client);
    return 0;
}