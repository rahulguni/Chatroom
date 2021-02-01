#pragma once
#include <string>
#include <thread>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

class Client; 

typedef void(*MessageReceivedHandler)(std::string msg); 

class Client
{
public:

	Client();
	~Client();
	bool initWSA(); 
	void connectSock();
	void sendMsg(std::string txt);
	std::thread recvThread;
	void threadRecv();
	std::string username;
	bool joinChat = true;


private:
	SOCKET createSocket();
	std::string serverIP = "127.0.0.1";
	int serverPort = 1986;
	sockaddr_in hint;
	SOCKET serverSocket;		
	bool recvThreadRunning; 


};

