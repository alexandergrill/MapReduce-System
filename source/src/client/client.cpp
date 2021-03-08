/*
 * author:  Alexander Grill
 * file:    client.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "client.h"
#include "utils.h"

#include <string.h>
#include <time.h>

#include <algorithm>
#include <fstream>
#include <iostream>


using namespace std;



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
    cout << "Write Random Strings into File" << endl;
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
    if (worddic.find(value) == worddic.end()){
        return false;
    }
    return true;
}

bool Client::IP_IsValid(string ip)
{
    int c{0};
    char *ip_input = strtok(&ip[0], ".");
    while (ip_input != NULL)
    {
        string ipstring = ip_input;
        size_t pos;
        int num = stoi(ipstring, &pos);
        if (pos != ipstring.size())
        {
            return false;
        }
        if (num < 0 || num > 255)
        {
            return false;
        }
        c += 1;
        ip_input = strtok(NULL, ".");
    }
    if (c < 4)
    {
        return false;
    }
    return true;
}

void Client::Print(){
    for (auto &t : worddic){
        cout << t.first << " " << t.second << endl;
    }
}

void Client::Map(string filename){
    cout << "MAP" << endl;
    string fileline;
    fstream file;
    int counter{0};
    try{
        file.open(filename, ios::in);
        if (!file){
            cout << "File not created" << endl;
        }
        else{
            cout << "File found" << endl;
            while (!file.eof()){
                file >> fileline;
                fileline.erase(remove(fileline.begin(), fileline.end(), '.'), fileline.end());
                fileline.erase(remove(fileline.begin(), fileline.end(), ','), fileline.end());
                fileline.erase(remove(fileline.begin(), fileline.end(), '"'), fileline.end());
                fileline.erase(remove(fileline.begin(), fileline.end(), '!'), fileline.end());
                fileline.erase(remove(fileline.begin(), fileline.end(), '?'), fileline.end());
                bool isin = Search(fileline);
                if (isin == false){
                    worddic.insert(pair<string, int>(fileline, 1));
                    counter += 1;
                }
                else{
                    auto it = worddic.find(fileline);
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