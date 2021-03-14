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

#include <string.h>
#include <time.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;
using namespace rang;

bool Client::IPIsValid(string ip){
    int c{0};
    char *ip_input = strtok(&ip[0], ".");
    while (ip_input != NULL){
        string ipstring = ip_input;
        size_t pos;
        int num = stoi(ipstring, &pos);
        if (pos != ipstring.size()){
            return false;
        }
        if (num < 0 || num > 255){
            return false;
        }
        c += 1;
        ip_input = strtok(NULL, ".");
    }
    if (c < 4){
        return false;
    }
    return true;
}

bool Client::PORTIsValid(string port){
    size_t pos;
    int num = stoi(port, &pos);
    if (pos != port.size() || port.length() != 4){
        return false;
    }
    if (num < 0 || num > 65535){
        return false;
    }
    return true;
}

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

bool Client::Search(string value){
    if (mapdic.find(value) == mapdic.end()){
        return false;
    }
    return true;
}

string Client::ConvertMap(){
    string  dicstring = "";
    for (auto &t : mapdic){
        dicstring += t.first;
        dicstring += ",";
        dicstring += to_string(t.second);
        dicstring += ":";
    }
    return dicstring;
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
                bool isin = Search(line);
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