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
    std::string ipadress;
    std::string port;
    std::map<std::string, int> datamap;
    const char alphabet[52] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G',
        'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U',
        'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g',
        'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u',
        'v', 'w', 'x', 'y', 'z'};
    Client(std::string ip, std::string pr):ipadress{ip}, port{pr}{}
    std::string GetRandomString();
public :
    static Client *GetClient(std::string ip, std::string pr);
    std::map<std::string, int>* GetMap();
    std::string ConvertMap();
    int GetDataMapSize();
    void WriteIntoFile(int wordnum, std::string filename);
    void Map(std::string filename);
};

#endif