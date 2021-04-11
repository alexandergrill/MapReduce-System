/*
 * author:  Alexander Grill
 * file:    slaveserver.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

//includes
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

//namespaces
using namespace std;
using namespace rang;

//Methoden Definitionen

/*
-Name: GetSlaveServer
-Beschreibung: gibt ein Objekt vom Typ SlaveServer
zurück, wenn Port und IP Adresse valid sind
-Input: string ip, string pr
-Input: string ip, string port, string sport, mutex& mx
-Output: SlaveServer*    
*/
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

/*
-Name: GetDataMapSize
-Beschreibung: gibt die Länger der Map resultmap zurück
-Input: 
-Output: int       
*/
int SlaveServer::GetDataMapSize(){
    return resultmap.size();
}

/*
-Name: Shuffle
-Beschreibung: 2 Phase des Map-Reduce Systems
-Input: 
-Output: void   
*/
void SlaveServer::Shuffle(){
    unique_lock<mutex> uls{mux};
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
