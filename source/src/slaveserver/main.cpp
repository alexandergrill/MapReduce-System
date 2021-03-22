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

using namespace std;
using namespace asio::ip;

int main(int argc, char *argv[]){
    string ipadress = "127.0.0.1";
    string port = "1116";
    string serverport = "1113";
    string transportstring = "";
    int maxclient{4};

    CLI::App app("MapReduceSystem_SlaverServer");
    app.add_option("-i,--i", ipadress, "ipadress for the server");
    //app.add_option("-p,--p", port, "port to connect to")->required();
    //app.add_option("-s,--s", port, "serverport")->required();
    CLI11_PARSE(app, argc, argv);

    SlaveServer *sl = SlaveServer::GetSlaveServer(ipadress, port, serverport);

    try{
        tcp::endpoint ep{tcp::v4(),sl->GetServerPort()};
        asio::io_context cox;
        tcp::acceptor ap{cox, ep};
        if(sl != nullptr){
            while(true){
                ap.listen();
                cout << "Server is listening" << endl;
                try{
                    tcp::iostream strm{ap.accept()};
                    sl->SetClientCounter();
                    cout << "Client " << sl->GetClientCounter() << " has connected to server" << endl;
                    if(sl->GetClientCounter() <= maxclient){
                        string data = "";
                        strm >> data;
                        map<string, int>* clientmap = ConvertStringtoMap(data);
                        sl->AddList(clientmap);
                        if(sl->GetListLength() == 2){
                            sl->PrintList();
                        }
                    }
                    else{
                        cout << "The maximum of clients was reached" << endl;
                        break;
                    }
                }
                catch(...){
                    cerr << "Error" << endl;
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
    delete sl;
}
