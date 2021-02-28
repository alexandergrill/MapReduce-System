/*
 * author:  Alexander Grill
 * file:    utils.h
 * project: mapreduce
 * date:    27.02.2021
*/

#include <random>

using namespace std;

int Get_RandomNum(int start, int end){
    random_device rd;
    mt19937 gen{rd()};
    uniform_int_distribution<> dis{start, end};
    int num = dis(gen);
    return num;
}