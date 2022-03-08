/*
	Conner Hendricks
	COP4635 Project2
*/

#include "TCPClient.hpp"

TCPClient::TCPClient(){


}

void TCPClient::connection(){
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY; //set ip of address
	servAddr.sin_port = htons(PORT); //set port of address
	memset(servAddr.sin_zero, '\0', sizeof servAddr.sin_zero); // set last struct variable

	char *messageToServer = "Hello from client";

	if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket creation error");
		exit(EXIT_FAILURE);
	}

	cout << "Socket Created Succesfully, looking for server" << endl;

	if(connect(clientSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
		perror("connection failure!");
		exit(EXIT_FAILURE);
	}

	cout << "Socket connected to Server" << endl;

	//send(clientSocket, messageToServer, strlen(messageToServer), 0);

	// sendData(messageToServer);
	// cout << "hello message sentt" << endl;
	// recieveData();
	
	// valRead = read(clientSocket, buffer, 1024);
	// //string buffString = string(buffer);
	// cout << buffer << endl;

}

void TCPClient::sendData(char *messageToServer){
	send(clientSocket, messageToServer, strlen(messageToServer), 0);
}

void TCPClient::recieveData(){
	valRead = read(clientSocket, buffer, 1024);
	//string buffString = string(buffer);
	cout << buffer << endl;
	
}

int main(int argc, char **argv){

	TCPClient client;
	client.connection();
	client.sendData("Hello from Client");
	cout << "hello message sentt" << endl;
	client.recieveData();

	return 0;
}
