/*
	Conner Hendricks
	COP4635 Project2
*/

#include "TCPClient.hpp"

TCPClient::TCPClient(){
	clientSocket = 0;
	valRead = 0;


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
	send(clientSocket, messageToServer, strlen(messageToServer) + 1, 0); // + 1 for terminating byte
	//send(clientSocket, messageToServer, strlen(messageToServer) 0); // + 1 for terminating byte
}

void TCPClient::recieveData(){
	valRead = read(clientSocket, buffer, 1024);
	//string buffString = string(buffer);
	cout << buffer << endl;
	
}

int main(int argc, char **argv){

	char buffer[1024];
	string strBuffer;

	TCPClient client;
	client.connection();

	cout << "Please enter your message: " << endl;
	fgets(buffer, 1024, stdin); //fgets reads newline char and null terminating char
	buffer[strcspn(buffer, "\n")] = 0; // use strcspn (string c span) to remove it 'cause it isn't needed

	client.sendData(buffer);
	client.recieveData();

	return 0;
}
