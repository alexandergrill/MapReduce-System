/*
 * author:  Alexander Grill
 * file:    main.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "utils.h"
#include "slaveserver.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <asio.hpp>
#include <rang.hpp>
#include <CLI11.hpp>

#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace asio::ip;

int main(int argc, char *argv[]){
    string ipadress = "127.0.0.1";
    string port = "1116";
    string serverport = "1113";
    string transportstring = "";
    int maxclient{4};
    int threadcounter{0};
    

    CLI::App app("MapReduceSystem_SlaverServer");
    app.add_option("-i,--i", ipadress, "ipadress for the server");
    app.add_option("-p,--p", port, "port to connect to")->required();
    app.add_option("-s,--s", serverport, "serverport")->required();
    app.add_option("-c,--c", maxclient, "the maximum of clients");
    CLI11_PARSE(app, argc, argv);

    cout << fg::green << flush;
    auto file = spdlog::basic_logger_mt("file_logger", "log-File.txt");
    spdlog::set_default_logger(file);
    spdlog::flush_on(spdlog::level::info);
    auto console = spdlog::stderr_color_mt("slaveserver_logger");
    console->set_level(spdlog::level::trace);

    vector<thread> pool(maxclient);
    SlaveServer *sl = SlaveServer::GetSlaveServer(ipadress, port, serverport);

    try{
        tcp::endpoint ep{tcp::v4(),sl->GetServerPort()};
        asio::io_context cox;
        tcp::acceptor ap{cox, ep};
        if(sl != nullptr){
            while(true){
                ap.listen();
                cout << fg::green << flush;
                spdlog::get("client_logger")->info("server is listening");
                spdlog::get("file_logger")->info("server is listening";
                try{
                    tcp::iostream strm{ap.accept()};
                    sl->SetClientCounter();
                    cout << fg::green << flush;
                    spdlog::get("client_logger")->info("Client " << sl->GetClientCounter() << " has connected to server");
                    spdlog::get("file_logger")->info("Client " << sl->GetClientCounter() << " has connected to server");
                    if(sl->GetClientCounter() <= maxclient){
                        string data = "";
                        strm >> data;
                        map<string, int>* clientmap = ConvertStringtoMap(data);
                        cout << fg::green << flush;
                        spdlog::get("client_logger")->info("convert data from client to map");
                        spdlog::get("file_logger")->info("convert data from client to map");
                        sl->AddList(clientmap);
                        if(sl->GetListLength() == 2){
                            cout << fg::green << flush;
                            spdlog::get("client_logger")->info("call SHUFFLE function");
                            spdlog::get("file_logger")->info("call SHUFFLE function");
                            pool[threadcounter] = thread (&SlaveServer::Shuffle, &*sl);
                            threadcounter += 1;
                        }
                        delete clientmap;
                    }
                    else{
                        cout << fg::green << flush;
                        spdlog::get("client_logger")->info("the maximum of clients was reached");
                        spdlog::get("file_logger")->info("the maximum of clients was reached");
                        break;
                    }
                }
                catch(...){
                    cout << fg::red << flush;
                    spdlog::get("client_logger")->error("clients are not reachable");
                    spdlog::get("file_logger")->error("lients are not reachable");
                }
            }
        }
        else{
            cout << "Slaveserver is null" << endl;
        }
        
    }
    catch(...){
        cout << "Slaveserver is null" << endl;
    }
    for(auto& t : pool){
        t.join();
    }
    delete sl;
}
