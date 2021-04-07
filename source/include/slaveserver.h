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
#include <mutex>
#include <condition_variable>

class SlaveServer{
private:
    std::string ipadresse;
    std::string port;
    int clientcounter{0};
    unsigned short serverport;
<<<<<<< HEAD
    std::map<std::string, int> datatmap;
=======
    std::map<std::string, int> datamap;
>>>>>>> b50782ec0bfe9408454101a5af9d2520e4808572
    std::map<std::string, int> resultmap;
    std::list<std::map<std::string, int>>* maplist;
    std::mutex &mxss;
    SlaveServer(std::string ip, std::string pr, unsigned short spr, std::mutex& xss):ipadresse{ip}, port{pr}, serverport{spr}, mxss{xss}{
        maplist = new std::list<std::map<std::string, int>>();
    }
    int FindElementinDataMap(std::string value);
public:
    std::condition_variable slavEx;
    ~SlaveServer(){
        delete maplist;
    }
<<<<<<< HEAD
    static SlaveServer* GetSlaveServer(std::string ip, std::string port, std::string sport);
=======
    static SlaveServer* GetSlaveServer(std::string ip, std::string port, std::string lport, std::mutex& mx);
>>>>>>> b50782ec0bfe9408454101a5af9d2520e4808572
    unsigned short GetServerPort();
    std::map<std::string, int>* GetMap();
    int GetListLength();
    int GetClientCounter();
    void SetClientCounter();
    void AddList(std::map<std::string, int>* mapdic);
    void PrintList();
    void ShrinkDataMap();
    void Shuffle();
};

#endif