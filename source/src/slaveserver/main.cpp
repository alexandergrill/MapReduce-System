/*
 * author:  Alexander Grill
 * file:    main.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

//includes
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
#include <mutex>

//namespaces
using namespace std;
using namespace rang;
using namespace asio::ip;

int main(int argc, char *argv[]){
    //Variablen
    mutex mx;                           //Mutex Objekt
    string ipadress = "127.0.0.1";      //IP Adresse  des SlaveServers
    string port;                        //Port zu den jeweiligen MasterServer
    string serverport;                  //Port des SlaveServer
    string transportstring = "";        //Übertragungsstring
    string slaveservername;             //Name des SlaveServer
    int maxclient{4};                   //Max. Anzahl der Clients
    int threadcounter{0};               //Hilfvariable für Threads
    
    //Kommandozeilenparameter
    CLI::App app("MapReduceSystem_SlaveServer");
    app.add_option("-n,--n", slaveservername, "name for the slaveserver")->required();
    app.add_option("-i,--i", ipadress, "ipadress for the server");
    app.add_option("-p,--p", port, "port to connect to")->required();
    app.add_option("-s,--s", serverport, "serverport")->required();
    app.add_option("-c,--c", maxclient, "the maximum of clients");
    CLI11_PARSE(app, argc, argv);
    
    //SPDLOG
    cout << fg::green << flush;
    auto file = spdlog::basic_logger_mt("file_logger", "log-File.txt");
    spdlog::set_default_logger(file);
    spdlog::flush_on(spdlog::level::info);
    auto console = spdlog::stderr_color_mt("slaveserver_logger");
    console->set_level(spdlog::level::trace);
    
    vector<thread> pool(maxclient/2);
    SlaveServer *sl = SlaveServer::GetSlaveServer(ipadress, port, serverport, ref(mx));

    //Auf Port abhören um Daten von Clients zu empfangen
    if(sl != nullptr){
        tcp::endpoint ep{tcp::v4(), sl->GetServerPort()};
        asio::io_context cox;
        tcp::acceptor ap{cox, ep};
        while (sl->GetConnectionCounter() < maxclient){
            ap.listen();
            cout << fg::green << flush;
            spdlog::get("slaveserver_logger")->info("server is listening");
            spdlog::get("file_logger")->info("server is listening");

            tcp::iostream strm{ap.accept()};
            sl->SetConnectionCounter();
            cout << fg::green << flush;
            spdlog::get("slaveserver_logger")->info("client " + to_string(sl->GetConnectionCounter()) + " has connected to server");
            spdlog::get("file_logger")->info("client " + to_string(sl->GetConnectionCounter()) + " has connected to server");
            
            //Übertragungsstring in Map abspeichern
            string data = "";
            strm >> data;
            sl->ConvertStringtoMap(data);

            cout << fg::green << flush;
            spdlog::get("slaveserver_logger")->info("convert data from client to map");
            spdlog::get("file_logger")->info("convert data from client to map");
            
            //Thread mit der Funktion Shuffle wird gestartet
            if (sl->GetListLength() == 2){
                cout << fg::green << flush;
                pool[threadcounter] = thread(&SlaveServer::Shuffle, &*sl);
                threadcounter += 1;
            }
        }

        for (auto &t : pool){
            t.join();
        }
        Print(sl->GetMap());

        //setzten des Transportstrings
        transportstring = ConvertMaptoString(sl->GetMap());
        transportstring += sl->GetTableData();
        transportstring += slaveservername + "+1," + to_string(-1) + ":" + slaveservername + "+1," + to_string(sl->GetDataMapSize()) + ":";

        cout << fg::green << flush;
        spdlog::get("slaveserver_logger")->info("convert map to transportdata");
        spdlog::get("file_logger")->info("convert map to transportdata");

        char *slaveserver_ip = &ipadress[0];
        char *slaveserver_port = &port[0];

        //Verbindung zu MasterServer aufbauen, wenn erfolgreich, werden die Daten gesendet
        tcp::iostream tcpconnection{slaveserver_ip, slaveserver_port};

        if (tcpconnection){
            tcpconnection << transportstring << endl;
            
            cout << fg::green << flush;
            spdlog::get("slaveserver_logger")->info("send data to masterserver");
            spdlog::get("file_logger")->info("send data to masterserver");
        }
        else{
            cout << fg::red << flush;
            spdlog::get("slaveserver_logger")->error("masterserver are not reachable");
            spdlog::get("file_logger")->error("masterserver are not reachable");
        }
        tcpconnection.close();
    }

    else{
        cout << fg::red << flush;
        spdlog::get("slaveserver_logger")->error("check input parameter");
        spdlog::get("file_logger")->error("check input parameter");
    }
    
    delete sl;
}
