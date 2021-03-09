/*
 * author:  Alexander Grill
 * file:    main.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "client.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
/*
#include <asio.hpp>
#include <rang/rang.hpp>
#include <CLI11/CLI11.hpp>
int argc, char* argv[]*/

#include <iostream>

using namespace std;

int main(){
    string ipadress = "127.0.0.1a";
    string port = "111";

    Client* c = Client::GetClient(ipadress, port);
    if(c != nullptr){
        //c->WriteIntoFile(500000, "../src/client/clientfile.txt");
        //c->Map("../src/client/clientfile.txt");
    }
}