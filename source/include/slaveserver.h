/*
 * author:  Alexander Grill
 * file:    slaveserver.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef SLAVESERVER_H
#define SLAVESERVER_H

//includes
#include "masterserver.h"

#include <map>
#include <string>
#include <list>
#include <mutex>

//Klasse SlaveServer
class SlaveServer : public MasterServer{
private:
//Variablen
    std::string ipadress;   //IP Adresse des SlaveServers
    std::string port;       //PORT des jeweiligen Servers
//Konstruktor
    SlaveServer(std::string ip, std::string pr, unsigned short spr, std::mutex& mx): MasterServer(spr, mx){
        ipadress = ip;
        port = pr;
    }
public:
//Destruktor
    ~SlaveServer(){}
//Methoden
    static SlaveServer* GetSlaveServer(std::string ip, std::string port, std::string lport, std::mutex& mx);
    int GetDataMapSize();
    void Shuffle();
};

#endif