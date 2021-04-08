/*
 * author:  Alexander Grill
 * file:    masterserver.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "masterserver.h"
#include "utils.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <rang.hpp>

#include <map>
#include <iostream>
#include <mutex>

using namespace std;
using namespace rang;

MasterServer *MasterServer::GetMasterServer(string port, mutex &mx){
    bool portvalid;
    MasterServer *mas;

    portvalid = PORTIsValid(port);

    if (portvalid == false){
        std::cerr << "Port is invalid!" << std::endl;
        mas = nullptr;
    }
    else{
        mas = new MasterServer(stoul(port, nullptr, 0), mx);
    }
    return mas;
}

unsigned short MasterServer::GetServerPort(){
    return serverport;
}

map<string, int>* MasterServer::GetMap(){
    return &resultmap;
}

int MasterServer::GetClientCounter(){
    return clientcounter;
}

void MasterServer::SetClientCounter(){
    clientcounter += 1;
}

void MasterServer::AddList(map<string, int> *mapdic){
    maplist->push_back(*mapdic);
}

int MasterServer::GetListLength(){
    return maplist->size();
}

void MasterServer::InsertElementinMap(string value, int valuecnt){
    if (resultmap.empty()){
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

void MasterServer::Reduce(){
    unique_lock<mutex> uls{mxms};
    auto listiterator = maplist->begin();
    advance(listiterator, maplist->size() - 2);

    map<string, int> mapf = *listiterator;
    ++listiterator;
    map<string, int> maps = *listiterator;

    maplist->pop_front();
    maplist->pop_front();

    bool found = false;

    cout << fg::green << flush;
    spdlog::get("masterserver_logger")->info("call reduce function");
    spdlog::get("file_logger")->info("call shuffle function");

    for (map<string, int>::iterator t1 = mapf.begin(); t1 != mapf.end(); ++t1){
        for (map<string, int>::iterator t2 = maps.begin(); t2 != maps.end(); ++t2){
            if (t1->first == t2->first){
                InsertElementinMap(t1->first, t1->second + t2->second);
                maps.erase(t2);
                found = true;
                break;
            }
        }
        if (found == false){
            InsertElementinMap(t1->first, t1->second);
        }
        found = false;
    }
    for (map<string, int>::iterator t3 = maps.begin(); t3 != maps.end(); ++t3){
        InsertElementinMap(t3->first, t3->second);
    }
}