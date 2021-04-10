/*
 * author:  Alexander Grill
 * file:    main.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "masterserver.h"
#include "utils.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <asio.hpp>
#include <rang.hpp>
#include <CLI11.hpp>

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;
using namespace rang;
using namespace asio::ip;

int main(int argc, char *argv[]){
    mutex mx;
    string port;
    string jsonfile;
    string masterservername;
    int maxslaveserver{2};
    int threadcounter{0};
    bool displaytable{false};

    CLI::App app("MapReduceSystem_MasterServer");
    app.add_option("-n,--n", masterservername, "name for the masterserver")->required();
    app.add_option("-p,--p", port, "serverport")->required();
    app.add_option("-c,--c", maxslaveserver, "the maximum of slaveserver");
    app.add_option("-j,--j", jsonfile, "write reduced data in json-file")->check(CLI::ExistingFile);
    app.add_flag("-t,--t", displaytable, "show you a table about the reduced data");
    CLI11_PARSE(app, argc, argv);

    cout << fg::green << flush;
    auto file = spdlog::basic_logger_mt("file_logger", "log-File.txt");
    spdlog::set_default_logger(file);
    spdlog::flush_on(spdlog::level::info);
    auto console = spdlog::stderr_color_mt("masterserver_logger");
    console->set_level(spdlog::level::trace);

    vector<thread> pool(maxslaveserver / 2);
    MasterServer* ma = MasterServer::GetMasterServer(port, ref(mx));

    
    if (ma != nullptr){
        tcp::endpoint ep{tcp::v4(), ma->GetServerPort()};
        asio::io_context cox;
        tcp::acceptor ap{cox, ep};
        while (ma->GetClientCounter() < maxslaveserver){
            ap.listen();
            cout << fg::green << flush;
            spdlog::get("masterserver_logger")->info("server is listening");
            spdlog::get("file_logger")->info("server is listening");

            tcp::iostream strm{ap.accept()};
            ma->SetClientCounter();
            cout << fg::green << flush;
            spdlog::get("masterserver_logger")->info("slaveserver " + to_string(ma->GetClientCounter()) + " has connected to server");
            spdlog::get("file_logger")->info("slaveserver " + to_string(ma->GetClientCounter()) + " has connected to server");

            string data = "";
            strm >> data;
            ma->ConvertStringtoMap(data);

            cout << fg::green << flush;
            spdlog::get("masterserver_logger")->info("convert data from slaveserver to map");
            spdlog::get("file_logger")->info("convert data from slaveserver to map");
            
            if (ma->GetListLength() == 2){
                pool[threadcounter] = thread(&MasterServer::Reduce, &*ma);
                threadcounter += 1;
            }
        }

        for (auto &t : pool){
            t.join();
        }
        Print(ma->GetMap());

        if (displaytable == true){
            ma->PrintTable(masterservername);
        }
        if(jsonfile.empty() == false){
            ma->WriteIntoFile(jsonfile);
        }
    }
    else{
        cout << fg::red << flush;
        spdlog::get("masterserver_logger")->error("check input parameter");
        spdlog::get("file_logger")->error("check input parameter");
    }

    delete ma;
}