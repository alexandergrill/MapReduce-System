/*
 * author:  Alexander Grill
 * file:    main.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "client.h"
#include "utils.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <asio.hpp>
#include <rang.hpp>
#include <CLI11.hpp>

#include <iostream>

using namespace std;
using namespace rang;
using namespace asio::ip;

int main(int argc, char* argv[]){
    int wordcount = 500000;
    string filepath = "../src/client/clientfile.txt";
    string ipadress = "127.0.0.1";
    string port;
    string clientname;
    string transportstring = "";
    
    CLI::App app("MapReduceSystem_Client");
    app.add_option("-n,--n", clientname, "name for the client")->required();
    app.add_option("-i,--i", ipadress, "ipadress for the client");
    app.add_option("-p,--p", port, "port to connect to")->required();
    app.add_option("-w,--w", wordcount, "wortcount how many random strings are used for the system");
    app.add_option("-f,--f", filepath, "filepath of the file");
    CLI11_PARSE(app, argc, argv);

    cout << fg::green << flush;
    auto file = spdlog::basic_logger_mt("file_logger", "log-File.txt");
    spdlog::set_default_logger(file);
    spdlog::flush_on(spdlog::level::info);
    auto console = spdlog::stderr_color_mt("client_logger");
    console->set_level(spdlog::level::trace);

    Client* c = Client::GetClient(ipadress, port);

    if(c != nullptr && wordcount > 0){

        char* client_ip = &ipadress[0];
        char* client_port = &port[0];

        tcp::iostream tcpconnection{client_ip, client_port};
            
        if(tcpconnection){
            cout << fg::green << flush;
            spdlog::get("client_logger")->info("established connection to server");
            spdlog::get("file_logger")->info("established connection to server");

            c->WriteIntoFile(wordcount, filepath);
            cout << fg::green << flush;
            spdlog::get("client_logger")->info("write random string into file");
            spdlog::get("file_logger")->info("write random string into file");

            c->Map(filepath);
            cout << fg::green << flush;
            spdlog::get("client_logger")->info("call map function, sort data in dictionary");
            spdlog::get("file_logger")->info("call map function, sort data in dictionary");

            Print(c->GetMap());
                    
            transportstring = ConvertMaptoString(c->GetMap());
            cout << fg::green << flush;
            spdlog::get("client_logger")->info("convert map to transportdata");
            spdlog::get("file_logger")->info("convert map to transportdata");
            transportstring += clientname + "0," + to_string(wordcount) + ":" + clientname + "1," + to_string(c->GetDataMapSize()) + ":";
            tcpconnection << transportstring << endl;

            cout << fg::green << flush;
            spdlog::get("client_logger")->info("send data to slaveserver");
            spdlog::get("file_logger")->info("send data to slaveserver");
             
            tcpconnection.close();  
        }
        else{
            cout << fg::red << flush;
            spdlog::get("client_logger")->error("server is not reachable");
            spdlog::get("file_logger")->error("server is not reachable");
            
        }
    }
    else{
        cout << fg::red << flush;
        spdlog::get("client_logger")->error("check input parameter");
        spdlog::get("file_logger")->error("check input parameter");
    }
    

    delete c;
}