/*
 * author:  Alexander Grill
 * file:    slaveserver.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef SLAVESERVER_H
#define SLAVESERVER_H

#include "masterserver.h"

#include <map>
#include <string>
#include <list>
#include <mutex>

class SlaveServer : MasterServer{
private:
    std::string ipadresse;
    std::string port;
    std::map<std::string, int> resultmap;
    std::list<std::map<std::string, int>>* maplist;

    SlaveServer(std::string ip, std::string pr, unsigned short spr, std::mutex& mx): Basis(spr, mx){
        ipadress = ip;
        port = pr;
    }
    ~SlaveServer(){
        delete maplist;
    }

public:
    static SlaveServer* GetSlaveServer(std::string ip, std::string port, std::string lport, std::mutex& mx);
    int GetDataMapSize();
    std::string GetClientsData();
    void Shuffle();
};

#endif