/*
 * author:  Alexander Grill
 * file:    slaveserver.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "slaveserver.h"
#include "utils.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <rang.hpp>

#include <map>
#include <iostream>
#include <mutex>
#include <sstream>

using namespace std;
using namespace rang;

SlaveServer* SlaveServer::GetSlaveServer(string ip, string port, string sport, mutex& mx){
    bool ipvalid;
    bool portvalid;
    bool povalid; 
    SlaveServer *sls;

    ipvalid = IPIsValid(ip);
    portvalid = PORTIsValid(port);
    povalid = PORTIsValid(sport);

    if (ipvalid == false || portvalid == false || povalid == false){
        cout << fg::red << flush;
        spdlog::get("slaveserver_logger")->error("IP Adress or Port is invalid!");
        spdlog::get("file_logger")->error("IP Adress or Port is invalid!");
        sls = nullptr;
    }
    else{
        sls = new SlaveServer(ip, port, stoul(sport, nullptr, 0), mx);
    }
    return sls;
}

unsigned short SlaveServer::GetServerPort(){
    return serverport;
}

map<string, int>* SlaveServer::GetMap(){
    return &resultmap;
}

int SlaveServer::GetClientCounter(){
    return clientcounter;
}

void SlaveServer::SetClientCounter(){
    clientcounter += 1;
}

void SlaveServer::AddList(map<string, int>* mapdic){
    maplist->push_back(*mapdic);
}

void SlaveServer::SetClientsData(std::string value, int valuecnt){
    clientsdata += value + "," + to_string(valuecnt) + ":";
}

string SlaveServer::GetClientsData(){
    return clientsdata;
}

int SlaveServer::GetDataMapSize(){
    return resultmap.size();
}

int SlaveServer::GetListLength(){
    return maplist->size();
}

void SlaveServer::InsertElementinMap(string value, int valuecnt){
    if(resultmap.empty()){
        resultmap.insert(pair<string, int>(value, valuecnt));
    }
    else{
        map<string, int>::iterator it;
        it = resultmap.find(value);
        if (it != resultmap.end()){
            resultmap.find(value)->second += valuecnt; 
        }
        else{
            resultmap.insert(pair<string, int>(value, valuecnt));
        }
    }
}

void SlaveServer::ConvertStringtoMap(std::string transportstr){
    map<string, int> *mapd = new map<string, int>();
    stringstream sstring(transportstr);
    string data;
    string mapelementdata;
    int mapelementcounter;
    int cnt{0};
    while (getline(sstring, data, ':')){
        int n = data.length();
        char *str = new char[n + 1];
        strcpy(str, data.c_str());
        char *strelement;
        strelement = strtok(str, ",");
        while (strelement != NULL){
            if (cnt == 0){
                mapelementdata = strelement;
            }
            else{
                mapelementcounter = stoi(strelement);
            }
            cnt += 1;
            strelement = strtok(NULL, ",");
        }
        cnt = 0;

        if (any_of(mapelementdata.begin(), mapelementdata.end(), ::isdigit)){
            SetClientsData(mapelementdata, mapelementcounter);
        }
        else{
            mapd->insert(pair<string, int>(mapelementdata, mapelementcounter));
        }
        delete str;
    }
    AddList(mapd);
}

void SlaveServer::Shuffle(){
    unique_lock<mutex> uls{mxss};
    auto listiterator = maplist->begin();
    advance(listiterator, maplist->size()-2);

    map<string, int> mapf = *listiterator;
    ++listiterator;
    map<string, int> maps = *listiterator;

    maplist->pop_front();
    maplist->pop_front();
    
    bool found = false;

    cout << fg::green << flush;
    spdlog::get("slaveserver_logger")->info("call shuffle function");
    spdlog::get("file_logger")->info("call shuffle function");

    for (map<string, int>::iterator t1 = mapf.begin(); t1 != mapf.end(); ++t1){
        for (map<string, int>::iterator t2 = maps.begin(); t2 != maps.end(); ++t2){
            if(t1->first == t2->first){
                InsertElementinMap(t1->first, t1->second + t2->second);
                maps.erase(t2);
                found = true;
                break;
            }
        }
        if(found == false){
            InsertElementinMap(t1->first, t1->second);
        }
        found = false;
    }
    for (map<string, int>::iterator t3 = maps.begin(); t3 != maps.end(); ++t3){
        InsertElementinMap(t3->first, t3->second);
    }
}
