/*
 * author:  Alexander Grill
 * file:    utils.h
 * project: mapreduce
 * date:    27.02.2021
*/

#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <iostream>

int GetRandomNum(int start, int end);
bool IPIsValid(std::string i);
bool PORTIsValid(std::string p);
bool Search(std::string value, std::map<std::string, int>* dic);
std::string ConvertMaptoString(std::map<std::string, int>* dic);
std::map<std::string, int>* ConvertStringtoMap(std::string transportstr);

#endif