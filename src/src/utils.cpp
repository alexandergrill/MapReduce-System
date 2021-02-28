/*
 * author:  Alexander Grill
 * file:    utils.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "utils.h"

#include <random>

using namespace std;

int GetRandomNum(int start, int end)
{
    random_device rd;
    mt19937 gen{rd()};
    uniform_int_distribution<> dis{start, end};
    int num = dis(gen);
    return num;
}