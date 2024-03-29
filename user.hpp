#ifndef USER_HPP
#define USER_HPP


#include <iostream>
#include <thread>

/*
 * C includes
 */

#include <string.h>
#include <bits/stdc++.h>
#include <sys/sendfile.h>


/*
 * Namespaces
 */

using namespace std;

#define PORT 60069

class User{

private:
	string userName;
	vector <string> locations;
	vector <char *> locationVector;
	string location, message;
	int socketNum, spotInVector;
	vector <char *> messagesVector;
	vector <string> messages;

public:
	User();
	User(char *userName, int socketNum);
	void setUserName(string userName);
	string getUserName();
	void setSocketNum(int socketNum);
	int getSocketNum();
	void setSpotInVector(int spotInVector);
	int getSpotInVector();
	void subToLocation(string location);
	bool checkForLocation(string location);
	void unsubToLocation(string location);
	vector<string> getLocations();
	void printLocations();
	void addMessage(string message);
	void printMessages();
	vector<string> getMessages();
	

};
#endif