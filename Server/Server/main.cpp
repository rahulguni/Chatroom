#include <iostream>
#include <string>
#include "server.h"

int main() {
	
	Server server;

	int serverStatus = server.initWSA();
	if (serverStatus != 0) {
		return -1;
	}

	server.start();


	return 0;

}

