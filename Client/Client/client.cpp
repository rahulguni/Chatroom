#include "client.h"
#include <iostream>
#include <string>
#include <thread>

using namespace std;



Client::Client()
{
	recvThreadRunning = false; 
}


Client::~Client()
{
	closesocket(serverSocket); 
	WSACleanup();
	if (recvThreadRunning) {
		recvThreadRunning = false;
		recvThread.join();	
	}
}


bool Client::initWSA() {
	
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cout << "Error: can't start Winsock." << endl;
		return false;
	}
	return true;
}

SOCKET Client::createSocket() {

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "Error: can't create socket." << endl;
		WSACleanup();
		return -1;
	}

	
	hint.sin_family = AF_INET;
	hint.sin_port = htons(serverPort);
	inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

	return sock;

}

void Client::threadRecv() {

	recvThreadRunning = true;
	while (recvThreadRunning) {

		char buf[4096];
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(serverSocket, buf, 4096, 0);	
		if (bytesReceived > 0) {			

			std::cout << string(buf, 0, bytesReceived) << std::endl;

		}

	}
}

void Client::connectSock() {

	

	serverSocket = createSocket();

	int connResult = connect(serverSocket, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cout << "Error: can't connect to server." << endl;
		closesocket(serverSocket);
		WSACleanup();
		return;
	}

}

void Client::sendMsg(string txt) {

	if (!txt.empty() && serverSocket != INVALID_SOCKET) {

		send(serverSocket, txt.c_str(), txt.size() + 1, 0);

		
		
	}

}
