/*
 * author:  Alexander Grill
 * file:    slaveserver.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef SLAVESERVER_H
#define SLAVESERVER_H

#include <map>
#include <string>
#include <list>
#include <mutex>

class SlaveServer{
private:
    std::string ipadresse;
    std::string port;
    int clientcounter{0};
    unsigned short serverport;
    std::map<std::string, int> resultmap;
    std::list<std::map<std::string, int>>* maplist;
    std::mutex &mxss;
    SlaveServer(std::string ip, std::string pr, unsigned short spr, std::mutex& xss):ipadresse{ip}, port{pr}, serverport{spr}, mxss{xss}{
        maplist = new std::list<std::map<std::string, int>>();
    }
    int FindElementinDataMap(std::string value);
public:
    ~SlaveServer(){
        delete maplist;
    }
    static SlaveServer* GetSlaveServer(std::string ip, std::string port, std::string lport, std::mutex& mx);
    unsigned short GetServerPort();
    std::map<std::string, int>* GetMap();
    int GetListLength();
    int GetClientCounter();
    void SetClientCounter();
    void AddList(std::map<std::string, int>* mapdic);
    void InsertElementinMap(std::string value, int valuecnt);
    void Shuffle();
};

#endif