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
	int socketNum;
	vector <char *> messages;

public:
	User();
	User(char *userName, int socketNum);
	void setUserName(string userName);
	string getUserName();
	void setSocketNum(int socketNum);
	int getSocketNum();
	void subToLocation(string location);
	void unsubToLocation(string location);
	vector<string> getLocations();
	void printLocations();
	

};
#endif