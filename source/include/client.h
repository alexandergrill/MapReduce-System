/*
 * author:  Alexander Grill
 * file:    client.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef CLIENT_H
#define CLIENT_H

//includes
#include <map>
#include <iostream>

//Klasse Client
class Client{
private:
//Variablen
    std::string ipadress;                   //IP Adresse des Clients
    std::string port;                       //PORT des jeweiligen Servers
    std::map<std::string, int> datamap;     //in dieser Map werden alle komprimierten Daten nach der Map Phase abgespeichert
    const char alphabet[52] = {             //Array dient für die Generierung von random strings
        'A', 'B', 'C', 'D', 'E', 'F', 'G',
        'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U',
        'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g',
        'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u',
        'v', 'w', 'x', 'y', 'z'};
//Konstruktor
    Client(std::string ip, std::string pr):ipadress{ip}, port{pr}{}
//Methoden
    std::string GetRandomString();
public :
//Methoden
    static Client *GetClient(std::string ip, std::string pr);
    std::map<std::string, int>* GetMap();
    int GetDataMapSize();
    void WriteIntoFile(int wordnum, std::string filename);
    int Map(std::string filename);
};

#endif