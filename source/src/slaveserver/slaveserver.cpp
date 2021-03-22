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
    list<map<string, int>>::iterator it = maplist->begin();
    advance(it, maplist->size()-1);
    map<string, int> mapdic1 = *it;
    advance(it, maplist->size()-2);
    map<string, int> mapdic2 = *it;
    bool isfound;
    for (auto &t1 : mapdic1){
        isfound = false;
        string worddic1 = t1.first;
        int countdic1 = t1.second;
        for(auto &t2 : mapdic2){
            string worddic2 = t2.first;
            int countdic2 = t2.second;
            if(worddic1.compare(worddic2)){
                isfound = true;
                resultmap.insert(pair<string,int>(worddic1, countdic1 + countdic2));
            }
        }
        if(isfound == false){
            resultmap.insert(pair<string,int>(worddic1, countdic1));
        }
    }

    for(auto &t3 : mapdic2){
        string worddic3 = t3.first;
        int countdic3 = t3.second;
        if (!resultmap.count(worddic3)){
             resultmap.insert(pair<string,int>(worddic3, countdic3));
        }
    }
}

void SlaveServer::PrintList(){
    int cnt{1};
    list<map<string, int>>::iterator itlist = maplist->begin();
    for (; itlist != maplist->end(); ++itlist){
        cout << cnt << " Map List" << endl;
        Print(&(*itlist));
    }
}