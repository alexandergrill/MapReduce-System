
/*
 * author:  Alexander Grill
 * file:    client.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef CLIENT_H
#define CLIENT_H


#include <map>
#include <iostream>


std::string GetRandomString();
void WriteIntoFile(int wordnum, std::string filename);
bool Search(std::string value);
void Print();
void Map(std::string filename);


#endif