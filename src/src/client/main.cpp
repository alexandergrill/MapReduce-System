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
    Client *c = new Client("127.0.0.1", "1113");
    c->WriteIntoFile(500000, "../src/client/clientfile.txt");
    c->Map("../src/client/clientfile.txt");
    delete(c);
} 