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

class SlaveServer : public MasterServer{
private:
    std::string ipadress;
    std::string port;


    SlaveServer(std::string ip, std::string pr, unsigned short spr, std::mutex& mx): MasterServer(spr, mx){
        ipadress = ip;
        port = pr;
    }
    

public:
    ~SlaveServer(){
    }
    static SlaveServer* GetSlaveServer(std::string ip, std::string port, std::string lport, std::mutex& mx);
    int GetDataMapSize();
    void Shuffle();
};

#endif