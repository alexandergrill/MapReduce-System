/*
 * author:  Alexander Grill
 * file:    slaveserver.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "slaveserver.h"
#include "utils.h"

#include <map>
#include <iostream>


using namespace std;

SlaveServer* SlaveServer::GetSlaveServer(std::string ip, std::string port, std::string sport){
    bool ipvalid;
    bool portvalid;
    bool povalid;
    SlaveServer *ssl;

    ipvalid = IPIsValid(ip);
    portvalid = PORTIsValid(port);
    povalid = PORTIsValid(sport);

    if (ipvalid == false || portvalid == false || povalid == false){
        std::cerr << "IP Adress or Port is invalid!" << std::endl;
        ssl = nullptr;
    }
    else{
        ssl = new SlaveServer(ip, port, stoul(sport, nullptr, 0));
    }
    return ssl;
}

unsigned short SlaveServer::GetServerPort(){
    return serverport;
}

int SlaveServer::GetClientCounter(){
    return clientcounter;
}

void SlaveServer::SetClientCounter(){
    clientcounter += 1;
}

void SlaveServer::AddList(std::map<std::string, int>* mapdic){
    maplist->push_back(*mapdic);
}

int SlaveServer::GetListLength(){
    return maplist->size();
}

void SlaveServer::Shuffle(){
    auto listiterator = maplist->begin();
    advance(listiterator, maplist->size()-2);

    map<string, int> mapf = *listiterator;
    ++listiterator;
    map<string, int> maps = *listiterator;

    maplist->pop_front();
    maplist->pop_front();
    
    bool found = false;
    
    for (map<string, int>::iterator t1 = mapf.begin(); t1 != mapf.end(); ++t1){
        for (map<string, int>::iterator t2 = maps.begin(); t2 != maps.end(); ++t2){
            if(t1->first == t2->first){
                datatmap.insert(pair<string, int>(t1->first, t1->second + t2->second));
                maps.erase(t2);
                found = true;
                break;
            }
        }
        if(found == false){
            datatmap.insert(pair<string, int>(t1->first, t1->second));
        }
        found = false;
    }
    for (map<string, int>::iterator t3 = maps.begin(); t3 != maps.end(); ++t3){
        datatmap.insert(pair<string, int>(t3->first, t3->second));
    }
    cout << "finished\n" << flush;
}

void SlaveServer::PrintList(){
    int cnt{1};
    list<map<string, int>>::iterator itlist = maplist->begin();
    for (; itlist != maplist->end(); ++itlist){
        cout << cnt << " Map from List" << endl;
        Print(&(*itlist));
        cnt += 1;
    }
}