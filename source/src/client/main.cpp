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

/*int argc, char* argv[]*/
//c->WriteIntoFile(500000, "../src/client/clientfile.txt");
//c->Map("../src/client/clientfile.txt");

#include <iostream>

using namespace std;
using namespace asio::ip;

int main(){
    string ipadress = "127.0.0.1";
    string port = "1113";
    string test = "HULLOWorld";

    Client* c = Client::GetClient(ipadress, port);
    if(c != nullptr){
        char* my_ip = &ipadress[0];
        char* my_port = &port[0];

        tcp::iostream tcpconnection{my_ip, my_port};

        if(tcpconnection){
            try{
                tcpconnection << test << endl;
                //getline(tcpconnection, test);
            }
            catch(...){
                cerr << "Error" << endl;
            }
        } 
        tcpconnection.close();  
    }
}