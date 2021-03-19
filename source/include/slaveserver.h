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

class Slaveserver{
private:
    std::string ipadresse;
    std::string port;
    unsigned short listenport;
    std::map<std::string, int> resultmapdic;
    std::list<std::map<std::string, int>> mapdiclist;
    Slaveserver(){}
    Slaveserver(std::string ip, std::string pr, unsigned short lpr):ipadresse{ip}, port{pr}, listenport{lpr}{}
public:
    static Slaveserver* GetSlaveServer(std::string ip, std::string port, std::string lport);
    unsigned short GetListenPort();
    int GetLengthList();
    void AddList(std::map<std::string, int> mapdic);
    void Shuffle();
};

#endif