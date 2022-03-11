#include "user.hpp"

User::User(){

}

void User::setUserName(char *userName){
    this->userName = userName;
}

string User::getUserName(){
    return userName;
}

void User::subToLocation(char *location){
    locations.push_back(location);
}

void User::unsubToLocation(char *location){

}

void User::getLocations(){

}