/*
 * author:  Alexander Grill
 * file:    utils.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef UTILS_H
#define UTILS_H

//includes
#include <map>
#include <iostream>

//Funktionen
int GetRandomNum(int start, int end);
bool IPIsValid(std::string i);
bool PORTIsValid(std::string p);
void Print(std::map<std::string, int>* dic);
bool Search(std::string value, std::map<std::string, int>* dic);
std::string ConvertMaptoString(std::map<std::string, int>* dic);

#endif