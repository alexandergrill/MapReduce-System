/*
 * author:  Alexander Grill
 * file:    masterserver.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef MASTERSERVER_H
#define MASTERSERVER_H

#include <map>
#include <string>
#include <list>
#include <mutex>


class MasterServer{
private:
    std::map<std::string, std::map<std::string, std::string>> clients;
    std::map<std::string, std::string> slaveserver;
    
    void SetMaps();
protected:
    int maxcounter{0};
    unsigned short serverport;
    std::string tabledata = "";
    std::mutex &mux;
    std::map<std::string, int> resultmap;
    std::list<std::map<std::string, int>> *maplist;
    
    MasterServer(unsigned short pr, std::mutex &mx) : serverport{pr}, mxms{mx}{
        maplist = new std::list<std::map<std::string, int>>();
    }
    ~MasterServer(){
        delete maplist;
    }

    int FindElementinDataMap(std::string value);
    void AddList(std::map<std::string, int> *mapdic);
    void SetClientsData(std::string value, int valuecnt);
public :
    
    static MasterServer* GetMasterServer(std::string port, std::mutex &mx);
    unsigned short GetServerPort();
    std::map<std::string, int>* GetMap();
    void PrintTable(std::string masterservername);
    int GetListLength();
    int GetClientCounter();
    void SetClientCounter();

    void ConvertStringtoMap(std::string transportstr);
    void InsertElementinMap(std::string value, int valuecnt);
    void WriteIntoFile(std::string jsonfile);
    void Reduce();
};

#endif