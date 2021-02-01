#include <iostream>
#include "client.h"
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main() {

	Client *client = new Client; 
	string msg = "a";
	string username; 

	cout << "==============================================" << endl;
	cout << "            WELCOME TO TCP CHATROOM           " << endl;
	cout << "==============================================" << endl;


	cout << "\n\nUsername: " << endl;
	cin >> username;
	cout << endl;
	client->username = username; 

	if (client->initWSA()) {

		client->connectSock();

		client->recvThread = thread([&] {
			client->threadRecv(); 
		});

		while (true) {
			getline(cin, msg); 
			std::string message; 
			if (client->joinChat == false) {
				std::ostringstream ss;
				ss << "[" << client->username << "]$  " << msg;
				message = ss.str();
			}
			else if (client->joinChat == true) {
				std::ostringstream ss; 
				ss << client->username << " is here, say hello!\n"; 
				message = ss.str(); 
				client->joinChat = false; 
			}
			client->sendMsg(message);
		}

	}


	delete client; 
	cin.get(); 
	return 0;

}