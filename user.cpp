/*
 * Conner Hendricks
 * COP4635 Project 2
 */

#include "user.hpp"

User::User(){

}

User::User(char *userName, int socketNum){

}

void User::setUserName(string userName){
    this->userName = userName;
}

string User::getUserName(){
    return userName;
}

void User::setSocketNum(int socketNum){
    this->socketNum = socketNum;
}

int User::getSocketNum(){
    return socketNum;
}

void User::subToLocation(string location){
    locations.push_back(location);
}

void User::unsubToLocation(string location){
    for(size_t i = 0; i < locations.size(); i++){
        if(locations[i] == location){
            locations[i].erase();
        }
    }
}

vector<string> User::getLocations(){
    return locations;
}

void User::printLocations(){
    for(size_t i = 0; i < locations.size(); i++){
        cout << locations[i] << endl;
    }
}