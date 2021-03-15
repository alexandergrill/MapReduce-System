/*
 * author:  Alexander Grill
 * file:    client.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "client.h"
#include "utils.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <rang/rang.hpp>


#include <time.h>

#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;
using namespace rang;

Client* Client::GetClient(std::string ip, std::string pr){
    bool ipvalid;
    bool povalid;
    Client *cl;

    ipvalid = IPIsValid(ip);
    povalid = PORTIsValid(pr);
    cout << fg::green << flush;
    spdlog::get("client_logger")->info("IP Adresse is valid " + to_string(ipvalid));
    spdlog::get("file_logger")->info("IP Adresse is valid " + to_string(ipvalid));
    cout << fg::green << flush;
    spdlog::get("client_logger")->info("PORT is valid " + to_string(povalid));
    spdlog::get("file_logger")->info("PORT is valid " + to_string(povalid));

    if (ipvalid == false || povalid == false){
        std::cerr << "IP Adress or Port is invalid!" << std::endl;
        cl = nullptr;
    }
    else{
        cl = new Client(ip, pr);
    }
    return cl;
}

map<string, int>* Client::GetMap(){
    return &mapdic;
}

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

void Client::WriteIntoFile(int wordnum, string filename){
    ofstream file;
    try{
        file.open(filename, ios::out);
        if (!file){
            cout << "File not created" << endl;
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
        cout << "Error" << endl;
    }
}



void Client::Print(){
    cout << fg::yellow << "Map Data Dictionary" << endl;
    for (auto &t : mapdic){
        cout << fg::yellow << t.first << " " << t.second << endl;
    }
}

void Client::Map(string filename){
    string line;
    fstream file;
    int counter{0};
    try{
        file.open(filename, ios::in);
        if (!file){
            //cout << "File not created" << endl;
        }
        else{
            //cout << "File found" << endl;
            while (!file.eof()){
                file >> line;
                bool isin = Search(line, &mapdic);
                if (isin == false){
                    mapdic.insert(pair<string, int>(line, 1));
                    counter += 1;
                }
                else{
                    auto it = mapdic.find(line);
                    it->second = it->second + 1;
                }
            }
            file.close();
            Print();
            cout << "Counter: " << counter << endl;
        }
    }
    catch (...){
        cout << "Can not find the file" << endl;
    }
}