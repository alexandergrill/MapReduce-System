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

int main(){
    string ipadress = "127.0.0.1";
    string port = "1113";
    string listenport = "1116";
    string transportstring = "";
    
    try{
        Slaveserver* sl = Slaveserver::GetSlaveServer(ipadress, port, listenport);
        tcp::endpoint ep{tcp::v4(),sl->GetListenPort()};
        asio::io_context cox;
        tcp::acceptor ap{cox, ep};
        if(sl != nullptr){
            while(true){
                ap.listen();
                cout << "Server is listening" << endl;
                try{
                    tcp::iostream strm{ap.accept()};
                    string data = "";
                    strm >> data;
                    map<string, int>* mapclient = ConvertStringtoMap(data);
                    sl->AddList(&mapclient);
                    if(sl->GetLengthList() == 2){
                        cout << "HELLO" << endl;
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
        delete sl;
    }
    catch(...){
        cout << "Slaveserver is null" << endl;
    }
    
}

