/*
 * author:  Alexander Grill
 * file:    client.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "client.h"
#include "utils.h"

#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;

map<std::string, int> worddic;

const char alphabet[52] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u',
    'v', 'w', 'x', 'y', 'z'};

string GetRandomString(){
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

void WriteIntoFile(int wordnum, string filename){
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

bool Search(string value){
    if (worddic.find(value) == worddic.end()){
        return false;
    }
    return true;
}

void Print(){
    for (auto &t : worddic){
        cout << t.first << " " << t.second << endl;
    }
}

void Map(string filename){
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