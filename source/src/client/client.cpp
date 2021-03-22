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

#include <rang.hpp>


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

map<string, int>* Client::GetMap(){
    return &datamap;
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
            cout << fg::red << flush;
            spdlog::get("client_logger")->error("File is not found");
            spdlog::get("file_logger")->error("File is not found");
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
        spdlog::get("client_logger")->error("It is not possible to write into file");
        spdlog::get("file_logger")->error("It is not possible to write into file");
    }
}

void Client::Map(string filename){
    string line;
    fstream file;
    int counter{0};
    try{
        file.open(filename, ios::in);
        if (!file){
            cout << fg::red << flush;
            spdlog::get("client_logger")->error("File is not found");
            spdlog::get("file_logger")->error("File is not found");
        }
        else{
            cout << fg::green << flush;
            spdlog::get("client_logger")->info("File is found");
            spdlog::get("file_logger")->info("File is found");

            while (!file.eof()){
                file >> line;
                bool isin = Search(line, &datamap);
                if (isin == false){
                    datamap.insert(pair<string, int>(line, 1));
                    counter += 1;
                }
                else{
                    auto it = datamap.find(line);
                    it->second = it->second + 1;
                }
            }
            file.close();
            cout << "Elements: " << counter << endl;
        }
    }
    catch (...){
        cout << fg::red << flush;
        spdlog::get("client_logger")->error("It is not possible to open file");
        spdlog::get("file_logger")->error("It is not possible to open file");
    }
}