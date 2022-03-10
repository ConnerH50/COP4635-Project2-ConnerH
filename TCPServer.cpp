#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable> //May not need
#include <fstream>

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

#include "user.hpp"

/*
 * Namespaces
 */

using namespace std;

#define PORT 60069
char httpHeader[25] = "HTTP/1.1 200 Ok\r\n"; //needed for sending stuff

char userName[1024] = {0};
char passWord[1024] = {0};

/*
 * Methods
 */


void sendData(int newSocket, char *message, int bufferSize){

	send(newSocket, message, strlen(message), bufferSize);

}

void recieveData(int newSocket, char *buffer, int bufferSize){
	cout << "in recieveData" << endl;

	//read(newSocket, buffer, bufferSize);
	//return recv(newSocket, buffer, bufferSize, 0);
	recv(newSocket, buffer, bufferSize, 0);
	int i = 0;

	// while((buffer[i++] = getchar()) != '\n'){
	// 	;
	// }
}

void getUserName(int socket, char *buffer){
	cout << "in getUserName" << endl;
	char userNameMessage[] = "\tUsername:";
	sendData(socket, userNameMessage, strlen(userNameMessage));
	recieveData(socket, userName, sizeof(userName));
	//return userName;
}

void getPassword(int socket, char *buffer){
	char passwordMessage[] = "\tPassword:";
	sendData(socket, passwordMessage, 0);
	recieveData(socket, passWord, sizeof(passWord));
	//return passWord;
}

bool login(int socket, char *buffer){

	ofstream loginFile;

	getUserName(socket, buffer);
	getPassword(socket, buffer);

	cout << "Username: " << userName << endl;
	cout << "Password: " << passWord << endl;

	return false;
}

bool registerForService(int socket, char *buffer){
	getUserName(socket, buffer);
	getPassword(socket, buffer);

	cout << "Username: " << userName << endl;
	cout << "Password: " << passWord << endl;

	return false;
}

void runServer(int socket, char *buffer){
	char welcomeMessage[] = "Welcome!\n\tPress 1 to Login\n\tPress 2 to Register\n\tType 'exit' to Quit\n";
	sendData(socket, welcomeMessage, 0);

	//cout << "in runServer" << endl;

	//main server loop
	recieveData(socket, buffer, strlen(buffer));
	while(1){
		cout << "in runServer while loop" << endl;
		//cout << "Buffer: " << buffer << endl;

		// if((strcmp(buffer, "exit") == 0)){
		// 	break;
		// }else 

		if(strcmp(buffer, "1") == 0){
			login(socket, buffer);

			//send rest of data

			sendData(socket, welcomeMessage, 0); // change this to long user message
			bzero(buffer, sizeof(buffer));
			recieveData(socket, buffer, sizeof(buffer));
		}else if(strcmp(buffer, "2") == 0){
			if(registerForService(socket, buffer) == true){
				char thankForRegistering[] = "Thank you for registering!\n\n";
				sendData(socket, thankForRegistering, 0);
				sendData(socket, welcomeMessage, 0);
				bzero(buffer, sizeof(buffer));
				recieveData(socket, buffer, sizeof(buffer));
			}else{
				char registerFailed[] = "I'm sorry, registering failed! Please try again!\n\n";
				sendData(socket, registerFailed, 0);
				sendData(socket, welcomeMessage, 0);
				bzero(buffer, sizeof(buffer));
				recieveData(socket, buffer, sizeof(buffer));
			}			
		}else{
			sendData(socket, welcomeMessage, 0);
			bzero(buffer, sizeof(buffer));
			recieveData(socket, buffer, sizeof(buffer));
		}
		
	}
}

void threadHandler(int socket, char *buffer){
	//thread newThread(runServer, newSocket, buffer);
	runServer(socket, buffer);
}

//server gets user data for client and sends it to client

int main(int argc, char **argv){
	int newSocket, serverFD, pid;
	int option = 1;
	char hostBuffer[1024];
	char buffer[1024];
	char *IPBuffer;
	struct hostent *hostEntry;
	struct sockaddr_in sockAddress;
	int addressLength = sizeof(sockAddress);

	char *messageToClient = "Hello from server\n";
	
	//get host things
	gethostname(hostBuffer, sizeof(hostBuffer)); //get host name
	hostEntry = gethostbyname(hostBuffer); //get host information. Needed to get IP address
	IPBuffer = inet_ntoa(*((struct in_addr*)hostEntry->h_addr_list[0])); //convert network into string

	// set address variables
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_addr.s_addr = INADDR_ANY; //set ip of address
	sockAddress.sin_port = htons(PORT); //set port of address
	memset(sockAddress.sin_zero, '\0', sizeof sockAddress.sin_zero); // set last struct variable


	// create serverFD w. error checking
	if((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Error cannot create socket");
		return -1;
	}

	setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	//Bind the serverFD
	if(bind(serverFD, (struct sockaddr *)&sockAddress, sizeof(sockAddress)) < 0){
		perror("Error in bind");
		return -1;
	}
	cout << "Server bind successful" << endl;

	//listen on socket
	if(listen(serverFD, 10) < 0){
		perror("error in listen");
		return -1;
	}

	cout << "Port is: " << PORT << endl;
	cout << "Host Name is: " << hostBuffer << endl;
	cout << "Host IP is: " << IPBuffer << endl << endl;

	while(1){
		if((newSocket = accept(serverFD, (struct sockaddr *)&sockAddress, (socklen_t*)&addressLength)) < 0){
				cout << "Error in accept testing" << endl;
				exit(EXIT_FAILURE);
		}

		cout << "accept completed" << endl << endl;

		//put threads here
		vector<thread> allClients;

		//char welcomeMessage[] = "Welcome!\n\tPress 1 to Login\n\tPress 2 to Register\n\tType 'exit' to Quit\n";
		//sendData(newSocket, welcomeMessage, 0);
		//runServer(newSocket, buffer);

		thread newThread(runServer, newSocket, buffer);
		// thread newThread(threadHandler, newSocket, buffer);
		newThread.detach();
	}

	//newThread.join();

	// if((pid = fork()) < 0){
	// 	cout << "Error in fork" << endl;
	// 	exit(EXIT_FAILURE);
	// }

	// if(pid == 0){ 
	// 	cout << "accept completed" << endl << endl;
	// 	while(1){
	// 		runServer(newSocket, buffer);
	// 	}
	// }else{

	// }

	//cout << "Username: " << userName << " Password: " << passWord << endl;

	cout << "Server Exiting" << endl;
	close(serverFD);

	return 0;
}
