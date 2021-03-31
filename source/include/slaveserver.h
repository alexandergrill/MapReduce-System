/*
 * author:  Alexander Grill
 * file:    slaveserver.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef SLAVESERVER_H
#define SLAVESERVER_H

#include <map>
#include <iostream>
#include <list>

class SlaveServer{
private:
    std::string ipadresse;
    std::string port;
    int clientcounter{0};
    unsigned short serverport;
     std::map<std::string, int> datatmap;
    std::map<std::string, int> resultmap;
    std::list<std::map<std::string, int>>* maplist;
    SlaveServer(std::string ip, std::string pr, unsigned short spr):ipadresse{ip}, port{pr}, serverport{spr}{
        maplist = new std::list<std::map<std::string, int>>();
    }
public:
    ~SlaveServer(){
        delete maplist;
    }
    static SlaveServer* GetSlaveServer(std::string ip, std::string port, std::string lport);
    unsigned short GetServerPort();
    int GetListLength();
    int GetClientCounter();
    void SetClientCounter();
    void AddList(std::map<std::string, int>* mapdic);
    void PrintList();
    void Shuffle();
};

#endif