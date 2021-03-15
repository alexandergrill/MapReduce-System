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
#include <vector>

class Slaveserver{
private:
    std::vector<std::map<std::string, int>> mapdiclist;
    std::string port;
    Slaveserver(){}
    Slaveserver(std::string pr):port{pr}{}
public:
    static Slaveserver* GetSlaveServer(std::string pr);
    void AddList(std::map<std::string, int> mapdic);
    void Splitt(std::map<std::string, int> mapdic1, std::map<std::string, int> mapdic2);
};

#endif