/*
 * author:  Alexander Grill
 * file:    main.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "client.h"
#include <iostream>

using namespace std;

int main(){
    WriteIntoFile(500000, "../src/client/clientfile.txt");
    Map("../src/client/clientfile.txt");
}