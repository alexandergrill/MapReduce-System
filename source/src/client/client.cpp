/*
 * author:  Alexander Grill
 * file:    client.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

//includes
#include "utils.h"
#include "client.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <rang.hpp>
#include <time.h>

#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

//namespaces
using namespace std;
using namespace rang;

//Methoden Definitionen

/*
-Name: GetRandomString
-Beschreibung: generiert eine zufälligen String und gibt diesen zurück
-Input: 
-Output: string       
*/
string Client::GetRandomString(){
    int wordlength = GetRandomNum(1, 5);
    int wordletter = GetRandomNum(0, 51);
    string word = "";
    for (int i = 0; i < wordlength; i++){
        word = word + alphabet[wordletter];
        wordletter = GetRandomNum(0, 51);
    }
    word += " ";
    return word;
}

/*
-Name: GetClient
-Beschreibung: gibt ein Objekt vom Typ Client
zurück, wenn Port und IP Adresse valid sind
-Input: string ip, string pr
-Output: Client*     
*/
Client* Client::GetClient(string ip, string pr){
    bool ipvalid;
    bool povalid;
    Client *cl;

    ipvalid = IPIsValid(ip);
    povalid = PORTIsValid(pr);
    cout << fg::green << flush;
    spdlog::get("client_logger")->info("IP Adress is valid " + to_string(ipvalid));
    spdlog::get("file_logger")->info("IP Adress is valid " + to_string(ipvalid));
    cout << fg::green << flush;
    spdlog::get("client_logger")->info("PORT is valid " + to_string(povalid));
    spdlog::get("file_logger")->info("PORT is valid " + to_string(povalid));

    if (ipvalid == false || povalid == false){
        cout << fg::red << flush;
        spdlog::get("client_logger")->error("IP Adress or Port is invalid!");
        spdlog::get("file_logger")->error("IP Adress or Port is invalid!");
        cl = nullptr;
    }
    else{
        cl = new Client(ip, pr);
    }
    return cl;
}

/*
-Name: GetMap
-Beschreibung: gibt einen Pointer des Objekts datamap zurück
-Input: 
-Output: map<string, int>*     
*/
map<string, int>* Client::GetMap(){
    return &datamap;
}

/*
-Name: GetDataMapSize
-Beschreibung: gibt die Länge der Map zurück
-Input: 
-Output: int      
*/
int Client::GetDataMapSize(){
    return datamap.size();
}

/*
-Name: WriteIntoFile
-Beschreibung: dabei werden zufällige String(Anzahl = wordnum)
in die Datei(filename) geschrieben und abgespeichert
-Input: int wordnum, string filename
-Output: void    
*/
void Client::WriteIntoFile(int wordnum, string filename){
    ofstream file;
    try{
        file.open(filename, ios::out);
        if (!file){
            cout << fg::red << flush;
            spdlog::get("client_logger")->error("file not found");
            spdlog::get("file_logger")->error("file not found");
        }
        else{
            for (int i = 0; i < wordnum; i++){
                string word = GetRandomString();
                file << word;
                if (i % 30 == 0){
                    file << endl;
                }
            }
        }
        file.close();
    }
    catch (...){
        cout << fg::red << flush;
        spdlog::get("client_logger")->error("it is not possible to write into file");
        spdlog::get("file_logger")->error("it is not possible to write into file");
    }
}

/*
-Name: Map
-Beschreibung: 1 Phase des Map-Reduce Systems
-Input: string filename
-Output: void    
*/
int Client::Map(string filename){
    int cnt{0};
    string line;
    fstream file;
    try{
        file.open(filename, ios::in);
        if (!file){
            cout << fg::red << flush;
            spdlog::get("client_logger")->error("file not found");
            spdlog::get("file_logger")->error("file not found");
        }
        else{
            cout << fg::green << flush;
            spdlog::get("client_logger")->info("file found");
            spdlog::get("file_logger")->info("file found");

            while (!file.eof()){
                file >> line;
                cnt += 1;
                line.erase(remove(line.begin(), line.end(), '.'),line.end());
                line.erase(remove(line.begin(), line.end(), ','), line.end());
                line.erase(remove(line.begin(), line.end(), '!'), line.end());
                line.erase(remove(line.begin(), line.end(), '?'), line.end());
                line.erase(remove(line.begin(), line.end(), ';'), line.end());
                line.erase(remove(line.begin(), line.end(), ':'), line.end());
                line = regex_replace(line, regex("„"),"");
                line = regex_replace(line, regex("“"), "");
                line.erase(remove(line.begin(), line.end(), '"'), line.end());

                bool isin = Search(line, &datamap);
                if (isin == false){
                    datamap.insert(pair<string, int>(line, 1));

                }
                else{
                    auto it = datamap.find(line);
                    it->second = it->second + 1;
                }
            }
            file.close();
        }
    }
    catch (...){
        cout << fg::red << flush;
        spdlog::get("client_logger")->error("it is not possible to open file");
        spdlog::get("file_logger")->error("it is not possible to open file");
    }
    return cnt-1;
}