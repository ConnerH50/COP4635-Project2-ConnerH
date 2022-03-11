#include "user.hpp"

User::User(){

}

User::User(char *userName, int socketNum){
    
}

void User::setUserName(char *userName){
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

void User::subToLocation(char *location){
    locations.push_back(location);
}

void User::unsubToLocation(char *location){

}

void User::getLocations(){

}