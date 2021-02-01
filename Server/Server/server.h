#pragma once

#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

class Server; 


typedef void(*MessageReceivedHandler)(Server* listener, int socketID, std::string msg);

class Server {
public:
	Server(); 
	Server(std::string ipAddress, int port);
	~Server(); 

	void sendMsg(int clientSocket, std::string msg);
	int initWSA(); 
	void start();
	void cleanupWinsock(); 
	

private: 
	SOCKET createSocket(); 
	std::string ipAddress;	
	int port; 
	int iResult;
	
};