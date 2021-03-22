/*
 * author:  Alexander Grill
 * file:    utils.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "utils.h"
#include <string.h>

#include <map>
#include <random>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int GetRandomNum(int start, int end){
    random_device rd;
    mt19937 gen{rd()};
    uniform_int_distribution<> dis{start, end};
    int num = dis(gen);
    return num;
}

bool IPIsValid(string ip){
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

bool PORTIsValid(string port){
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

bool Search(string value, std::map<std::string, int>* dic){
    if (dic->find(value) == dic->end()){
        return false;
    }
    return true;
}

string ConvertMaptoString(std::map<std::string, int>* dic){
    string dicstring = "";

    for (map<string, int>::iterator i = dic->begin(); i != dic->end(); ++i){
        dicstring += i->first;
        dicstring += ",";
        dicstring += to_string(i->second);
        dicstring += ":";
    }
    return dicstring;
}

void Print(std::map<std::string, int>* dic){
    cout << "Data Map Dictionary" << endl;
    for (map<string, int>::iterator t = dic->begin(); t != dic->end(); ++t){
        cout << "Data " << t->first << " " << t->second << endl;
    }
}

map<string, int>* ConvertStringtoMap(std::string transportstr){
    map<string, int>* mapd = new map<string, int>();
    vector<string> stringdata;
    stringstream ss(transportstr);
    string data;
    while(getline(ss, data, ':')){
        stringdata.push_back(data);
    }
    return mapd;
 }