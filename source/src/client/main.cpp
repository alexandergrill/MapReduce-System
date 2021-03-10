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


//c->WriteIntoFile(500000, "../src/client/clientfile.txt");
//c->Map("../src/client/clientfile.txt");

#include <iostream>

using namespace std;
using namespace asio::ip;

int main(int argc, char* argv[]){
    int wordcount = 500000;
    string filepath = "../src/client/clientfile.txt";
    string ipadress = "127.0.0.1";
    string port = "1113";
    string transportstring = "";
    
    CLI::App app("MapReduceSystem_Client");
    app.add_option("-i,--i", ipadress, "ipadress for the client");
    app.add_option("-p,--p", port, "port to connect to");
    app.add_option("-w,--w", wordcount, "wortcount how many random strings are used for the system");
    app.add_option("-f,--f", filepath, "filepath where are the data stored");
    CLI11_PARSE(app, argc, argv);

    Client* c = Client::GetClient(ipadress, port);

    try{
        if(c != nullptr && wordcount > 0){
            char* my_ip = &ipadress[0];
            char* my_port = &port[0];

            tcp::iostream tcpconnection{my_ip, my_port};

         if(tcpconnection){
                try{
                    tcpconnection << transportstring << endl;
                }
                catch(...){
                    cerr << "Error" << endl;
                }
            } 
            tcpconnection.close();  
        }
        else{
            throw -1;
        }
    }
    catch(int e){
        cerr << "client object has not been created, check input parameter" << endl;
        return -1;
    }
}