/*
 * author:  Alexander Grill
 * file:    main.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

//includes
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

//namespaces
using namespace std;
using namespace rang;
using namespace asio::ip;

int main(int argc, char *argv[]){
    //Variablen
    mutex mx;                   //Mutex Objekt
    string port;                //Port des Master Servers
    string jsonfile;            //Pfad des JSON-Files
    string masterservername;    //Name des MasterServers
    int maxslaveserver{2};      //Max. Anzahl der SlaveServer
    int threadcounter{0};       //Hilfvariable für Threads
    bool displaytable{false};   //bool ob Tabelle geprinted werden soll

    //Kommandozeilenparameter
    CLI::App app("MapReduceSystem_MasterServer");
    app.add_option("-n,--n", masterservername, "name for the masterserver")->required();
    app.add_option("-p,--p", port, "serverport")->required();
    app.add_option("-c,--c", maxslaveserver, "the maximum of slaveserver");
    app.add_option("-j,--j", jsonfile, "write reduced data in json-file")->check(CLI::ExistingFile);
    app.add_flag("-t,--t", displaytable, "print a table about the reduced data");
    CLI11_PARSE(app, argc, argv);

    //SPDLOG
    cout << fg::green << flush;
    auto file = spdlog::basic_logger_mt("file_logger", "log-File.txt");
    spdlog::set_default_logger(file);
    spdlog::flush_on(spdlog::level::info);
    auto console = spdlog::stderr_color_mt("masterserver_logger");
    console->set_level(spdlog::level::trace);

    vector<thread> pool(maxslaveserver / 2);
    MasterServer* ma = MasterServer::GetMasterServer(port, ref(mx));

    //Auf Port abhören um Daten von SlaveServer zu empfangen
    if (ma != nullptr){
        tcp::endpoint ep{tcp::v4(), ma->GetServerPort()};
        asio::io_context cox;
        tcp::acceptor ap{cox, ep};
        while (ma->GetConnectionCounter() < maxslaveserver){
            ap.listen();
            cout << fg::green << flush;
            spdlog::get("masterserver_logger")->info("server is listening");
            spdlog::get("file_logger")->info("server is listening");

            tcp::iostream strm{ap.accept()};
            ma->SetConnectionCounter();
            cout << fg::green << flush;
            spdlog::get("masterserver_logger")->info("slaveserver " + to_string(ma->GetConnectionCounter()) + " has connected to server");
            spdlog::get("file_logger")->info("slaveserver " + to_string(ma->GetConnectionCounter()) + " has connected to server");

            //Übertragungsstring in Map abspeichern
            string data = "";
            strm >> data;
            ma->ConvertStringtoMap(data);

            cout << fg::green << flush;
            spdlog::get("masterserver_logger")->info("convert data from slaveserver to map");
            spdlog::get("file_logger")->info("convert data from slaveserver to map");
            
            //Thread mit der Funktion Reduce wird gestartet
            if (ma->GetListLength() == 2){
                pool[threadcounter] = thread(&MasterServer::Reduce, &*ma);
                threadcounter += 1;
            }
        }

        for (auto &t : pool){
            t.join();
        }

        Print(ma->GetMap());
        
        //Ausgabe der Tabelle auf der Konsole
        if (displaytable == true){
            ma->PrintTable(masterservername);
        }
        //Daten werden nach der Reduce Phase JSON-File abgespeichert
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