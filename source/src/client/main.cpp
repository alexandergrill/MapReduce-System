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

#include <asio.hpp>
#include <rang/rang.hpp>
#include <CLI11/CLI11.hpp>

#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    string ipadress = "127.0.0.1";
    string port = "1113";

    Client* c = Client::GetClient(ipadress, port);
    if(c != NULL){
        c->WriteIntoFile(500000, "../src/client/clientfile.txt");
        c->Map("../src/client/clientfile.txt");
    }
}