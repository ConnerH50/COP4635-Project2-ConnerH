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

void User::unsubToLocation(char *location){

}

vector<string> User::getLocations(){
    return locations;
}

void User::printLocations(){
    for(int i = 0; i < locations.size(); i++){
        cout << locations[i] << endl;
    }
}