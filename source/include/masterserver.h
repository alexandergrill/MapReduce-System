/*
 * author:  Alexander Grill
 * file:    masterserver.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef MASTERSERVER_H
#define MASTERSERVER_H

//includes
#include <map>
#include <string>
#include <list>
#include <mutex>

//Klasse MasterServer
class MasterServer{
private:
//Variablen
    std::map<std::string, std::map<std::string, std::string>> clients;      //map dient zur Ausgabe der Tabelle(tabulate)
    std::map<std::string, std::string> slaveserver;                         //map dient zur Ausgabe der Tabelle(tabulate)
//Methoden
    void SetMaps();
protected:
//Variablen
    int connectioncounter{0};                                               //max. Anzahl der SlaveServer
    unsigned short serverport;                                              //Port des MasterServers
    std::string tabledata = "";                                             //notwendiger Übertragungsstring für Tabelle(tabulate)
    std::mutex &mux;                                                        //Mutex Objekt
    std::map<std::string, int> resultmap;                                   //in dieser Map werden alle komprimierten Daten nach der Reduce Phase abgespeichert
    std::list<std::map<std::string, int>> *maplist;                         //Liste aller Maps(SlaveServer)
//Konstruktor
    MasterServer(unsigned short pr, std::mutex &mx) : serverport{pr}, mux{mx}{
        maplist = new std::list<std::map<std::string, int>>();
    }
//Methoden
    void AddList(std::map<std::string, int> *mapdic);
    void SetTableData(std::string value, int valuecnt);
public:
//Destruktor
    ~MasterServer(){
        delete maplist;
    }
//Methoden
    std::string GetTableData();
    static MasterServer* GetMasterServer(std::string port, std::mutex &mx);
    unsigned short GetServerPort();
    std::map<std::string, int>* GetMap();
    int GetListLength();
    int GetConnectionCounter();
    void SetConnectionCounter();
    void ConvertStringtoMap(std::string transportstr);
    void InsertElementinMap(std::string value, int valuecnt);
    void PrintTable(std::string masterservername);
    void WriteIntoFile(std::string jsonfile);
    void Reduce();
};

#endif