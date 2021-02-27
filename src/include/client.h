
/*
 * author:  Alexander Grill
 * file:    client.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef CLIENT_H
#define CLIENT_H

#include <map>
#include <fstream>
#include <iostream>


//std::map<std::string, int>

std::map<std::string, int> worddic;

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
    std::string fileline;
    std::fstream file;
    try{
        file.open(filename, std::ios::in);
        if(!file){
            std::cout << "File not created" << std::endl;
        }
        else{
            std::cout << "File found" << std::endl;
            while(!file.eof()){
                file >> fileline;
                bool isin = Search(fileline);
                if(isin == false){
                    worddic.insert(std::pair<std::string, int>(fileline, 1));
                }
                else{
                    auto it = worddic.find(fileline);
                    it->second = it->second + 1;
                }
            }
            file.close();
            Print();
        }

    }
    catch(...){
        std::cout << "Can not find the file" << std::endl;
    }

}


#endif