#include "server.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

const int MAX_BUFFER_SIZE = 4096;			


Server::Server() {
	ipAddress = "127.0.0.1";
	port = 1986;
}


Server::~Server() {
	cleanupWinsock();		
}


int Server::initWSA() {

	WSAData wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << endl; 
		return -1;
	}

	return 0;

}


SOCKET Server::createSocket() {

	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	

	if (listenSocket != INVALID_SOCKET) {

		sockaddr_in hint;		
		hint.sin_family = AF_INET;	
		hint.sin_port = htons(port);	
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		iResult = bind(listenSocket, (sockaddr*)&hint, sizeof(hint));	

		if (iResult == SOCKET_ERROR) {			
			cout << "bind failed with error " << WSAGetLastError() << endl;
			return -1;
		}

		int iResult = listen(listenSocket, SOMAXCONN);	
		if (iResult == SOCKET_ERROR) {
			cout << "listen failed with error " << WSAGetLastError() << endl;
			return -1;
		}

		return listenSocket;

	}

}


void Server::start() {

	cout << "==============================================" << endl;
	cout << "               TCP CHATROOM SERVER            " << endl;
	cout << "==============================================\n" << endl;

	char buf[MAX_BUFFER_SIZE];	
	SOCKET listeningSocket = createSocket();		

	while (true) {

		if (listeningSocket == INVALID_SOCKET) {
			break;
		}

		fd_set master_socket;			
		FD_ZERO(&master_socket);			

		FD_SET(listeningSocket, &master_socket);		

		while (true) {

			fd_set master_socket_copy = master_socket;	
			int socketCount = select(0, &master_socket_copy, nullptr, nullptr, nullptr);			

			for (int i = 0; i < socketCount; i++) {			

				SOCKET sock = master_socket_copy.fd_array[i];				

				if (sock == listeningSocket) {			

					SOCKET clientSocket = accept(listeningSocket, nullptr, nullptr);		
					FD_SET(clientSocket, &master_socket);		
					string welcomeMsg = "Connected! Say hello to your friends.\n";		
					send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
					cout << "A new user has joined the chat." << endl;		

				}
				else {									

					ZeroMemory(buf, MAX_BUFFER_SIZE);	
					int bytesReceived = recv(sock, buf, MAX_BUFFER_SIZE, 0);	

					if (bytesReceived <= 0) {	
						closesocket(sock);
						FD_CLR(sock, &master_socket);	
					}
					else {						 
						for (int i = 0; i < master_socket.fd_count; i++) {			
							SOCKET outSock = master_socket.fd_array[i];	

							if (outSock != listeningSocket) {

								if (outSock == sock) {		
									string msgSent = "(Sent!)\n";
									send(outSock, msgSent.c_str(), msgSent.size() + 1, 0);	
								}
								else {						
									
									
									
									send(outSock, buf, bytesReceived, 0);		
								}

							}
						}

						cout << string(buf, 0, bytesReceived) << endl;			

					}

				}
			}
		}


	}

}



void Server::sendMsg(int clientSocket, string msg) {

	send(clientSocket, msg.c_str(), msg.size() + 1, 0);

}


void Server::cleanupWinsock() {

	WSACleanup();

}

