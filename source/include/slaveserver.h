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

class Slaveserver{
private:
    std::map<std::string, int> mapdic;
    std::string port;
    Slaveserver(){}
    Slaveserver(std::string pr):port{pr}{}
public:
    static Slaveserver* GetSlaveServer(std::string pr);
};

#endif