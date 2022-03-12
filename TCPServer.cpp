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
//ofstream loginFile;
fstream loginFile;
string fileName = "loginInfo.txt";
vector<User> userVector;
//User user; //not needed


char userName[1024] = {0};
char passWord[1024] = {0};

/*
 * Methods
 */


void sendData(int newSocket, char *message, int bufferSize){

	send(newSocket, message, strlen(message), bufferSize);

}

void recieveData(int newSocket, char *buffer, int bufferSize){
	// cout << "in recieveData" << endl;

	recv(newSocket, buffer, bufferSize, 0);
}

void getUserName(int socket){
	// cout << "in getUserName" << endl;
	char userNameMessage[] = "\tUsername:";
	sendData(socket, userNameMessage, strlen(userNameMessage));
	recieveData(socket, userName, sizeof(userName));
	//return userName;
}

void getPassword(int socket){
	char passwordMessage[] = "\tPassword:";
	sendData(socket, passwordMessage, 0);
	recieveData(socket, passWord, sizeof(passWord));
	//return passWord;
}

bool login(int socket, char *buffer/*, User user*/){

	// cout << "		SocketNum: " << socket << endl;

	//ifstream loginFile;

	//loginFile.open(fileName);

	getUserName(socket);
	getPassword(socket);

	// cout << "Username: " << userName << endl;
	// cout << "Password: " << passWord << endl;

	loginFile.open(fileName);
	if(loginFile.is_open()){
		//cout << "File is open" << endl;

		//use getline and parse the line
		//try and match username and password
		//if matched, make a new user object and return true
		//char *fileLine[1024];
		string fileLine;

		//get line from file
		while(getline(loginFile, fileLine)){
			//cout << "In getline" << endl << endl;
			//cout << fileLine << endl << endl;

			//tokenize string by ',' or ","
			stringstream sstream(fileLine);
			string token;
			string tokens[2];
			int i = 0;

			// tokenize string and store in array
			while(getline(sstream, token, ',')){
				tokens[i] = token;
				i++;
			}

			//Try and match username and password
			//cout << "	Username: " << userName << endl;
			//cout << "	Password:" << passWord << endl;

			// for(int i = 0; i < 2; i++){
			// 	cout << "	Token: " << tokens[i] << endl;
			// }

			if((strcmp(tokens[0].c_str(), userName) == 0) && (strcmp(tokens[1].c_str(), passWord) == 0)){
				cout << "		Username and password match!" << endl;
				// do user stuff
				//user.setUserName(userName);
				//user.setSocketNum(socket);
				loginFile.close();
				return true;
			}
		}

		loginFile.close();
		return false;
	}else{
		return false;
	}

	return false;
}

bool login(int socket, char *buffer, User user){

	// cout << "		SocketNum: " << socket << endl;

	//ifstream loginFile;

	//loginFile.open(fileName);

	getUserName(socket);
	getPassword(socket);

	// cout << "Username: " << userName << endl;
	// cout << "Password: " << passWord << endl;

	loginFile.open(fileName);
	if(loginFile.is_open()){
		//cout << "File is open" << endl;

		//use getline and parse the line
		//try and match username and password
		//if matched, make a new user object and return true
		//char *fileLine[1024];
		string fileLine;

		//get line from file
		while(getline(loginFile, fileLine)){
			//cout << "In getline" << endl << endl;
			//cout << fileLine << endl << endl;

			//tokenize string by ',' or ","
			stringstream sstream(fileLine);
			string token;
			string tokens[2];
			int i = 0;

			// tokenize string and store in array
			while(getline(sstream, token, ',')){
				tokens[i] = token;
				i++;
			}

			//Try and match username and password
			//cout << "	Username: " << userName << endl;
			//cout << "	Password:" << passWord << endl;

			// for(int i = 0; i < 2; i++){
			// 	cout << "	Token: " << tokens[i] << endl;
			// }

			if((strcmp(tokens[0].c_str(), userName) == 0) && (strcmp(tokens[1].c_str(), passWord) == 0)){
				cout << "		Username and password match!" << endl;
				// do user stuff
				user.setUserName(userName);
				user.setSocketNum(socket);

				loginFile.close();
				return true;
			}
		}

		loginFile.close();
		return false;
	}else{
		return false;
	}

	return false;
}

bool registerForService(int socket, char *buffer){

	getUserName(socket);
	getPassword(socket);

	// cout << "Username: " << userName << endl;
	// cout << "Password: " << passWord << endl;

	//loginFile.open(fileName);
	loginFile.open(fileName, ios::app); //ios::app for appending
	if(loginFile.is_open()){
		loginFile << userName << "," << passWord << endl;
		loginFile.close();
		return true;
	}else{
		return false;
	}
}

void subToLocation(User clientUser, char *buffer){
	cout << "socket: " << clientUser.getSocketNum() << endl;
	sendData(clientUser.getSocketNum(), "Where would you like to subscribe to?", 0);
	bzero(buffer, sizeof(buffer));
	recieveData(clientUser.getSocketNum(), buffer, sizeof(buffer));
	clientUser.subToLocation(buffer);

	//return false;
}

void runServer(int socket, char *buffer){
	cout << "socket: " << socket << endl;
	cout << "Client Handler Assigned" << endl;
	char welcomeMessage[] = "Welcome!\n\tPress 1 to Login\n\tPress 2 to Register\n\tType 'exit' to Quit\n";
	
	char loginString[] = "Logged In\n\t1: Subscribe to a location\n\t2: Unsubscribe from location\n\t3: Send message to location\n\t4: Send private message\n\t5: See subscribed locations\n\t6: See online users\n\t8: Change password\n\t0: 'exit' to Quit\n";
	bool loggedIn = false;
	User clientUser;

	//User user = new User();
	
	sendData(socket, welcomeMessage, 0);

	//cout << "in runServer" << endl;

	//main server loop
	recieveData(socket, buffer, strlen(buffer));
	while(1){
		// cout << "in runServer while loop" << endl;

		if(loggedIn == true){

			sendData(socket, loginString, 0); // change this to long user message

			bzero(buffer, sizeof(buffer));
			recieveData(socket, buffer, sizeof(buffer));

			if(strcmp(buffer, "1") == 0){ // sub to location
				cout << "In 1, " << "Username is: " << clientUser.getUserName() << endl << endl;

				subToLocation(clientUser, buffer);

				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));
			}else if(strcmp(buffer, "2") == 0){ //unsub
				cout << "In 2" << endl << endl;
				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));

			}else if(strcmp(buffer, "3") == 0){ //send message to location
				cout << "In 3" << endl << endl;
				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));
				
			}else if(strcmp(buffer, "4") == 0){ //send private message
				cout << "In 4" << endl << endl;
				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));

			}else if(strcmp(buffer, "5") == 0){ //see subbed locations
				cout << "In 5" << endl << endl;
				//clientUser.printLocations();

				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));
				
			}else if(strcmp(buffer, "6") == 0){ //see all online users
				cout << "In 6" << endl << endl;

				for(int i = 0; i < userVector.size(); i++){
					char userInVector[1024];
					strcpy(userInVector, userVector[i].getUserName().c_str());
					sendData(socket, userInVector, 0);
				}

				sendData(socket, "\n\n", 0);
				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));
				
			}else if(strcmp(buffer, "7") == 0){ //see last 10 messages
				cout << "In 7" << endl << endl;
				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));

			}else if(strcmp(buffer, "8") == 0){ //change password
				cout << "In 8" << endl << endl;
				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));
				
			}else{ // other
				// sendData(socket, loginString, 0);
				// bzero(buffer, sizeof(buffer));
				// recieveData(socket, buffer, sizeof(buffer));
			}
			
		}else{
				
			if(strcmp(buffer, "1") == 0){
				if(login(socket, buffer, clientUser) == true){
					string username = userName;
					loggedIn = true;
					cout << userName << endl;
					//cout << username << endl;
					clientUser.setUserName(username.c_str());
					clientUser.setSocketNum(socket);
					cout << "User's username: " << clientUser.getUserName() << endl;
					cout << "User's socket number: " << clientUser.getSocketNum() << endl;
					userVector.push_back(clientUser);
					//cout << loggedIn << endl;
				}else{
					char loginFailed[] = "I'm sorry, login failed! Please try again!\n\n";
					sendData(socket, loginFailed, 0);
					sendData(socket, welcomeMessage, 0);
					bzero(buffer, sizeof(buffer));
					recieveData(socket, buffer, sizeof(buffer));
				}
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

		cout << "Another Loop completed" << endl;
		
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
