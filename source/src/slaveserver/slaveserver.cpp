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

Slaveserver* Slaveserver::GetSlaveServer(std::string pr){
    bool povalid;
    Slaveserver *ssl;

    povalid = PORTIsValid(pr);
    /*cout << fg::green << flush;
    spdlog::get("client_logger")->info("IP Adresse is valid " + to_string(ipvalid));
    spdlog::get("file_logger")->info("IP Adresse is valid " + to_string(ipvalid));
    cout << fg::green << flush;
    spdlog::get("client_logger")->info("PORT is valid " + to_string(povalid));
    spdlog::get("file_logger")->info("PORT is valid " + to_string(povalid));*/

    if (povalid == false){
        std::cerr << "IP Adress or Port is invalid!" << std::endl;
        ssl = nullptr;
    }
    else{
        ssl = new Slaveserver(pr);
    }
    return ssl;
}

void Slaveserver::AddList(std::map<std::string, int> mapdic){
    mapdiclist.push_back(mapdic);
}

int Slaveserver::GetLengthList(){
    return mapdiclist.size();
}

void Slaveserver::Shuffle(){
    list<map<string, int>>::iterator it = mapdiclist.begin();
    advance(it, mapdiclist.size()-1);
    map<string, int> mapdic1 = *it;
    advance(it, mapdiclist.size()-2);
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
                resultmapdic.insert(pair<string,int>(worddic1, countdic1 + countdic2));
            }
        }
        if(isfound =  false){
            resultmapdic.insert(pair<string,int>(worddic1, countdic1));
        }
    }

    for(auto &t3 : mapdic2){
        string worddic3 = t3.first;
        int countdic3 = t3.second;
        if (!resultmapdic.count(worddic3)){
             resultmapdic.insert(pair<string,int>(worddic3, countdic3));
        }
    }
}