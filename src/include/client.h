
/*
 * author:  Alexander Grill
 * file:    client.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef CLIENT_H
#define CLIENT_H

#include "utils.h"

#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <time.h>

//std::map<std::string, int>

std::map<std::string, int> worddic;

const char alphabet[52] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u',
    'v', 'w', 'x', 'y', 'z'};


std::string GetRandomString()
{
    int wordlength = Get_RandomNum(1, 5);
    int wordletter = Get_RandomNum(0, 51);
    std::string word = "";
    for (int i = 0; i < wordlength; i++){
        word = word + alphabet[wordletter];
        wordletter = Get_RandomNum(0, 51);
    }    
    word += " ";
    return word;
}

void WriteIntoFile(int wordnum, std::string filename){
    std::cout << "Write Random Strings into File" << std::endl;
    std::ofstream file;
    try{
        file.open(filename, std::ios::out);
        if(!file){
            std::cout << "File not created" << std::endl;
        }
        else{
            for(int i = 0; i < wordnum; i++){
                std::string word = GetRandomString();
                file << word;
                if(i % 30 == 0){
                    file << std::endl;
                }
            }
        
        }
        file.close();
    }
    catch(...){

    }
}

bool Search(std::string value){
    if(worddic.find(value) == worddic.end()){
        return false;
    }
    return true;
}

void Print(){
    for(auto& t : worddic){
        std::cout << t.first << " " << t.second << std::endl;
    }
}


void Map(std::string filename){
    std::cout << "MAP" << std::endl;
    std::string fileline;
    std::fstream file;
    int counter{0};
    try{
        file.open(filename, std::ios::in);
        if(!file){
            std::cout << "File not created" << std::endl;
        }
        else{
            std::cout << "File found" << std::endl;
            while(!file.eof()){
                file >> fileline;
                fileline.erase(std::remove(fileline.begin(), fileline.end(), '.'), fileline.end());
                fileline.erase(std::remove(fileline.begin(), fileline.end(), ','), fileline.end());
                fileline.erase(std::remove(fileline.begin(), fileline.end(), '"'), fileline.end());
                fileline.erase(std::remove(fileline.begin(), fileline.end(), '!'), fileline.end());
                fileline.erase(std::remove(fileline.begin(), fileline.end(), '?'), fileline.end());
                bool isin = Search(fileline);
                if(isin == false){
                    worddic.insert(std::pair<std::string, int>(fileline, 1));
                    counter += 1;
                }
                else{
                    auto it = worddic.find(fileline);
                    it->second = it->second + 1;
                }
            }
            file.close();
            Print();
            std::cout << "Counter: " << counter << std::endl;
        }

    }
    catch(...){
        std::cout << "Can not find the file" << std::endl;
    }

}


#endif