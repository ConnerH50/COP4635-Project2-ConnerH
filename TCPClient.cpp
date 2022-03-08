/*
	Conner Hendricks
	COP4635 Project2
*/

#include "TCPClient.hpp"

void TCPClient::connection(){


}

void TCPClient::sendData(){

	
}

void TCPClient::recieveData(){

	
}

int main(int argc, char **argv){
	int clientSocket = 0;
	int valRead;
	struct sockaddr_in servAddr;
	struct hostent *hostEntry;
	char buffer[1024];

	char *messageToServer = "Hello from server";

	// set address variables
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY; //set ip of address
	servAddr.sin_port = htons(PORT); //set port of address
	memset(servAddr.sin_zero, '\0', sizeof servAddr.sin_zero); // set last struct variable

	if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket creation error");
		exit(EXIT_FAILURE);
	}

	if(connect(clientSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
		perror("connection failure!");
		exit(EXIT_FAILURE);
	}

	send(clientSocket, messageToServer, strlen(messageToServer), 0);
	cout << "hello message sentt" << endl;
	valRead = read(clientSocket, buffer, 1024);
	//string buffString = string(buffer);
	cout << buffer << endl;

	//cout << "Hi from Client" << endl;

	return 0;
}
