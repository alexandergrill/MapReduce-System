
/*
 * author:  Alexander Grill
 * file:    client.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef CLIENT_H
#define CLIENT_H

#include <map>
#include <iostream>

class Client{
private:
    const char alphabet[52] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G',
        'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U',
        'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g',
        'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u',
        'v', 'w', 'x', 'y', 'z'};
    std::map<std::string, int> worddic;
    std::string ipadress = "127.0.0.1";
    std::string port = "1113";

    bool IP_IsValid(std::string i);
    bool PORT_IsValid(std::string p);

public : 
    Client(){ 
    }
    Client(std::string ip, std::string pr){
        bool ipvalid;
        bool povalid;
        ipvalid = IP_IsValid(ip);
        povalid = PORT_IsValid(port);
        if(ipvalid == true && povalid){
            std::cerr << "IP Adress or Port is invalid!" << std::endl;
        }
        else{
            this->ipadress = ip;
            this->port = port;
        }
    }
    std::string GetRandomString();
    void WriteIntoFile(int wordnum, std::string filename);
    bool Search(std::string value);
    void Print();
    void Map(std::string filename);
};

#endif