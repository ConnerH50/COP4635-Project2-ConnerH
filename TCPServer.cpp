/*
 * Conner Hendricks
 * COP4635 Project 2
 */

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
#include <signal.h>

#include "user.hpp"

/*
 * Namespaces
 */

using namespace std;

#define PORT 60069
#define BUFFERSIZE 1024
//ofstream loginFile;
fstream loginFile;
string fileName = "loginInfo.txt";
vector<User> userVector;
vector<thread> threadVector;


char userName[1024] = {0};
char passWord[1024] = {0};
char newPassword[1024];
char location[1024];

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
}

void getPassword(int socket){
	char passwordMessage[] = "\tPassword:";
	sendData(socket, passwordMessage, 0);
	recieveData(socket, passWord, sizeof(passWord));
}

bool login(int socket, char *buffer){

	getUserName(socket);
	getPassword(socket);

	loginFile.open(fileName);
	if(loginFile.is_open()){
		string fileLine;

		//get line from file
		while(getline(loginFile, fileLine)){

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


			if((strcmp(tokens[0].c_str(), userName) == 0) && (strcmp(tokens[1].c_str(), passWord) == 0)){
				cout << "		Username and password match!" << endl;
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

	loginFile.open(fileName, fstream::app); //ios::app for appending
	if(loginFile.is_open()){
		loginFile << userName << "," << passWord << endl;
		loginFile.close();
		return true;
	}else{
		return false;
	}
}

void changePassword(User clientUser, char *buffer){
	char changePassMessage[] = "What would you like your new password to be?";
	sendData(clientUser.getSocketNum(), changePassMessage, sizeof(changePassMessage));
	bzero(buffer, sizeof(buffer));
	recieveData(clientUser.getSocketNum(), newPassword, sizeof(newPassword));

	string fileLine;
	vector<string> fileLines;
	loginFile.open(fileName, fstream::in | fstream::out);

	if(loginFile.is_open()){
		//getline into vector?
		while(getline(loginFile, fileLine)){
			cout << fileLine << endl << endl;
			fileLines.push_back(fileLine);

			//tokenize string by ","
			stringstream sstream(fileLine);
			string token;
			string tokens[2];
			int i = 0;

			// tokenize string and store in array
			while(getline(sstream, token, ',')){
				tokens[i] = token;
				i++;
			}

			if((strcmp(tokens[0].c_str(), clientUser.getUserName().c_str()) == 0)){
				string newUserPass = clientUser.getUserName() + "," + newPassword;
				cout << newUserPass << endl << endl;
				fileLines[fileLines.size() - 1] = newUserPass;
			}

		}
	}
	loginFile.close();

	loginFile.open(fileName, fstream::in | fstream::out);
	if(loginFile.is_open()){

		for(size_t i = 0; i < fileLines.size(); i++){
			loginFile << fileLines[i] << endl;
		}
	}

	loginFile.close();
}

void getLocation(User clientUser, char *buffer){
	char getLocationString[] = "What location?";
	sendData(clientUser.getSocketNum(), getLocationString, 0);
	bzero(buffer, sizeof(buffer));
	recieveData(clientUser.getSocketNum(), location, sizeof(location));
}

void getMessage(User clientUser, char *buffer){ // just like get userName, maybe should return the buffer?

}

void sendMessage(User clientUser, char *buffer){

}

void runServer(int socket, char *buffer){
	cout << "Client Handler Assigned" << endl;
	char welcomeMessage[] = "Welcome!\n\tPress 1 to Login\n\tPress 2 to Register\n\tType 'exit' to Quit\n";
	
	char loginString[] = "\n\n---------------------------------------\nLogged In\n\t1: Subscribe to a location\n\t2: Unsubscribe from location\n\t3: Send message to location\n\t4: Send private message\n\t5: See subscribed locations\n\t6: See online users\n\t8: Change password\n\t0: 'exit' to Quit\n";
	bool loggedIn = false;
	User clientUser;

	while(1){
		signal(SIGPIPE, SIG_IGN); //lets server handle SIGPIPE signal from client

		if(loggedIn == true){

			sendData(socket, loginString, 0); 

			bzero(buffer, sizeof(buffer));
			recieveData(socket, buffer, 1024);

			if(strcmp(buffer, "1") == 0){ // sub to location
				//cout << "In 1, " << "Username is: " << clientUser.getUserName() << endl << endl;

				getLocation(clientUser, buffer);
				string locationString = location;
				cout << "Location chosen: " << locationString << endl;
				clientUser.subToLocation(locationString);

			}else if(strcmp(buffer, "2") == 0){ //unsub
				//cout << "In 2" << endl << endl;

				getLocation(clientUser, buffer);
				string locationString = location;
				cout << "Location chosen: " << locationString << endl;
				clientUser.unsubToLocation(locationString);
			}else if(strcmp(buffer, "3") == 0){ //send message to location
				//cout << "In 3" << endl << endl;

			}else if(strcmp(buffer, "4") == 0){ //send private message
				//cout << "In 4" << endl << endl;

			}else if(strcmp(buffer, "5") == 0){ //see subbed locations
				//cout << "In 5" << endl << endl;
				vector<string> locationVector = clientUser.getLocations();

				char sendLocationString[] = "Subscribed Locations: \n\n";
				sendData(socket, sendLocationString, 0);

				for(size_t i = 0; i < locationVector.size(); i++){
					char userLocation[1024];
					strcpy(userLocation, locationVector[i].c_str());
					strcat(userLocation,"\n");
					sendData(socket, userLocation, 0);
				}

			}else if(strcmp(buffer, "6") == 0){ //see all online users, do something like this for seeing subscriptions
				//cout << "In 6" << endl << endl;
				char sendUsersString[] = "List of users: \n\n";
				sendData(socket, sendUsersString, 0);

				for(size_t i = 0; i < userVector.size(); i++){
					char userInVector[1024];
					strcpy(userInVector, userVector[i].getUserName().c_str());
					strcat(userInVector,"\n");
					sendData(socket, userInVector, 0);
				}

			}else if(strcmp(buffer, "7") == 0){ //see last 10 messages from client
				//cout << "In 7" << endl << endl;

			}else if(strcmp(buffer, "8") == 0){ //change password
				//cout << "In 8" << endl << endl;
				changePassword(clientUser, buffer);

			}else{ // other
				
			}
			
		}else{

			sendData(socket, welcomeMessage, 0);

			bzero(buffer, sizeof(buffer));
			recieveData(socket, buffer, sizeof(buffer));
				
			if(strcmp(buffer, "1") == 0){
				if(login(socket, buffer) == true){
					string usernameString = userName;
					loggedIn = true;
					
					clientUser.setUserName(usernameString);
					clientUser.setSocketNum(socket);
					userVector.push_back(clientUser);
				}else{
					char loginFailed[] = "I'm sorry, login failed! Please try again!\n\n";
					sendData(socket, loginFailed, 0);
				}
			}else if(strcmp(buffer, "2") == 0){
				if(registerForService(socket, buffer) == true){
					char thankForRegistering[] = "Thank you for registering!\n\n";
					sendData(socket, thankForRegistering, 0);
				}else{
					char registerFailed[] = "I'm sorry, registering failed! Please try again!\n\n";
					sendData(socket, registerFailed, 0);
				}			
			}else{
				
			}

		}
		
	}

}

void threadHandler(int socket, char *buffer){
	runServer(socket, buffer);
}

//server gets user data for client and sends it to client

int main(int argc, char **argv){
	int newSocket, serverFD;
	int option = 1;
	char hostBuffer[1024];
	char buffer[1024];
	char *IPBuffer;
	struct hostent *hostEntry;
	struct sockaddr_in sockAddress;
	int addressLength = sizeof(sockAddress);
	
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
		thread newThread(threadHandler, newSocket, buffer);
		newThread.detach();
	}

	close(serverFD);

	return 0;
}