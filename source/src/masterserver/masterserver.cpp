/*
 * author:  Alexander Grill
 * file:    masterserver.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

//includes
#include "masterserver.h"
#include "utils.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

//ignore warning "-Wnon-virtual-dtor" from extern library "tabulate"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include "tabulate.hpp"
#pragma GCC diagnostic pop

#include <json.hpp>
#include <rang.hpp>


#include <map>
#include <iostream>
#include <mutex>
#include <fstream>
#include <sstream>

//namespaces
using namespace std;
using namespace rang;
using namespace tabulate;
using json = nlohmann::json;

//Methoden Definitionen

/*
-Name: AddList
-Beschreibung: dabei werden die Daten des Strings tabledata,
der für die Ausgabe der Tabelle auf der Console von Relevant sind,
in den jeweiligen Maps abgespeichert
-Input: 
-Output: void    
*/
void MasterServer::SetMaps(){
    stringstream sstring(tabledata);
    string data;
    string clientservername = "";
    string datacnt1;
    string datacnt2;
    int cnt{0};
    while (getline(sstring, data, ':')){
        int n = data.length();
        char *str = new char[n + 1];
        strcpy(str, data.c_str());
        char *strelement;
        strelement = strtok(str, ",");
        while (strelement != NULL){
            if (cnt == 0){
                clientservername = strelement;
                cnt += 1;
            }
            else{
                if (cnt == 1){
                    datacnt1 = strelement;
                    cnt += 1;
                }
                else{

                    datacnt2 = strelement;
                    cnt += 1;
                }
            }
            strelement = strtok(NULL, ",");
            if (cnt == 4){
                cnt = 0;
                if (clientservername.find("+0") != string::npos){
                    clientservername.erase(clientservername.size() - 1);
                    clientservername.erase(clientservername.size() - 1);
                    clients[clientservername][datacnt1] = datacnt2;
                }
                else{
                    clientservername.erase(clientservername.size() - 1);
                    clientservername.erase(clientservername.size() - 1);
                    slaveserver[clientservername] = datacnt2;
                }
            }
        }
        delete str;
    }
}

/*
-Name: AddList
-Beschreibung: fügt eine neue Map in die Liste maplist hinzu
-Input: map<string, int> *mapdic
-Output: void      
*/
void MasterServer::AddList(map<string, int> *mapdic){
    maplist->push_back(*mapdic);
}

/*
-Name: SetTableData
-Beschreibung: erweitert den String tabledata
-Input: string value, int valuecnt
-Output: void
*/
void MasterServer::SetTableData(string value, int valuecnt){
    tabledata += value + "," + to_string(valuecnt) + ":";
}

/*
-Name: GetTableData
-Beschreibung: gibt den String tabledata zurück
-Input: 
-Output: string     
*/
string MasterServer::GetTableData(){
    return tabledata;
}

/*
-Name: GetMasterServer
-Beschreibung: gibt ein Objekt vom Typ MasterServer
zurück, wenn Port valid sind
-Input: string port, mutex &mx
-Output:  MasterServer*      
*/
MasterServer* MasterServer::GetMasterServer(string port, mutex &mx){
    bool portvalid;
    MasterServer *mas;

    portvalid = PORTIsValid(port);

    if (portvalid == false){
        cout << fg::red << flush;
        spdlog::get("masterserver_logger")->error("IP Adress or Port is invalid!");
        spdlog::get("file_logger")->error("IP Adress or Port is invalid!");
        mas = nullptr;
    }
    else{
        mas = new MasterServer(stoul(port, nullptr, 0), mx);
    }
    return mas;
}

/*
-Name: GetServerPort
-Beschreibung: gibt den Port des Servers zurück
-Input: 
-Output: unsigned short     
*/
unsigned short MasterServer::GetServerPort(){
    return serverport;
}

/*
-Name: GetMap
-Beschreibung: gibt den Pointer des Objekts resultmap zurück
-Input: 
-Output: map<string, int>*     
*/
map<string, int>* MasterServer::GetMap(){
    return &resultmap;
}

/*
-Name: GetListLength
-Beschreibung: gibt die Größe der Liste zurück
-Input: 
-Output: int       
*/
int MasterServer::GetListLength(){
    return maplist->size();
}

/*
-Name: GetConnectionCounter
-Beschreibung: gibt den Wert der Varible connectioncounter zurück
-Input: 
-Output: int       
*/
int MasterServer::GetConnectionCounter(){
    return connectioncounter;
}

/*
-Name: SetConnectionCounter
-Beschreibung: erhöht die Variable connectioncounter um 1
-Input: 
-Output: void       
*/
void MasterServer::SetConnectionCounter(){
    connectioncounter += 1;
}

/*
-Name: ConvertStringtoMap
-Beschreibung: konvertiert die Daten des Transport strings in eine Map
und fügt die angelegte Map in die Liste hinzu
-Input: 
-Output: void     
*/
void MasterServer::ConvertStringtoMap(string transportstr){
    map<string, int> *mapd = new map<string, int>();
    stringstream ss(transportstr);
    string data;
    string mapelementdata;
    int mapelementcounter;
    int cnt{0};
    while (getline(ss, data, ':')){
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
            SetTableData(mapelementdata, mapelementcounter);
        }
        else{
            mapd->insert(pair<string, int>(mapelementdata, mapelementcounter));
        }
        delete str;
    }
    AddList(mapd);
}

/*
-Name: InsertElementinMap
-Beschreibung: fügt Datensätze in die resultmap ein
-Input: 
-Output: void        
*/
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

/*
-Name: PrintTable
-Beschreibung: wird aufgerufen wenn Benutzer das Programm MasterServer
mit -t startet, dabei wird am Ende der Reduce Phase eine Tabelle in
der Console ausgegeben bezgl. den überarbeiteten Daten
-Input: string masterservername
-Output: void     
*/
void MasterServer::PrintTable(string masterservername){
    Table objects_table;
    objects_table.format().font_style({FontStyle::bold}).width(14);
    objects_table.add_row({"number of strings", "client", "map", "slaveserver", "shuffle", "masterserver", "reduce"});
    SetMaps();

    map<string, map<string, string>>::iterator itr;
    map<string, string>::iterator ptr;
    map<string, string>::iterator itr2;
    itr2 = slaveserver.begin();
    cout << "ClientMAP" << endl;
    int cnt{0};
    for (itr = clients.begin(); itr != clients.end(); itr++){
        for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++){
            if(cnt == 0){
                objects_table.add_row({ptr->first, itr->first, ptr->second, itr2->first, itr2->second, masterservername, to_string(resultmap.size())});
                itr2++;
                cnt = 1;
            }
            else if(itr2 != slaveserver.end()){
                objects_table.add_row({ptr->first, itr->first, ptr->second, itr2->first, itr2->second, "", ""});
                itr2++;
            }
            else{
                objects_table.add_row({ptr->first, itr->first, ptr->second, "", "", "", ""});
            }
            
        }
    }
    cout << objects_table << endl;
}

/*
-Name: WriteIntoFile
-Beschreibung: wird aufgerufen wenn Benutzer das Programm MasterServer
mit -j startet, dabei wird am Ende der Reduce Phase alle resultierenden
Daten in ein JSON File geschrieben
-Input: string jsonfile
-Output: void      
*/
void MasterServer::WriteIntoFile(string jsonfile){
    json data;
    ofstream of(jsonfile);
    for (map<string, int>::iterator t = resultmap.begin(); t != resultmap.end(); ++t){
        string value = t->first;
        int valuecounter = t->second;
        data["value"] = value;
        data["number"] = valuecounter;
        of << data << endl;
    }
}

/*
-Name: Reduce
-Beschreibung: 3 Phase des Map-Reduce Systems
-Input: 
-Output: void      
*/
void MasterServer::Reduce(){
    unique_lock<mutex> uls{mux};
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


