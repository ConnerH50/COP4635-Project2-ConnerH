#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP


#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable> //May not need

/*
 * C includes
 */

#include <string.h>
#include <bits/stdc++.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 * Namespaces
 */

using namespace std;

#define PORT 60069

class TCPClient{

private:
	int clientSocket;
	int valRead;
	struct sockaddr_in servAddr;
	struct hostent *hostEntry;
	char buffer[1024];
	char inputBuffer[1024];

public:
	TCPClient();
	void connection();
	void sendData(char *messageToServer);
	void recieveData();
	char *getBuffer();
	int getClientSocket();
	void runClient();
	void recieveMessage();
	

};
#endif
