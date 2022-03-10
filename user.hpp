#ifndef USER_HPP
#define USER_HPP


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

class User{

private:
	string userName;
	vector <string> locations;
	int socketNum;
	vector <string> messages;

public:
User();
	void setUserName(string userName);
	string getUserName();
	

};
#endif