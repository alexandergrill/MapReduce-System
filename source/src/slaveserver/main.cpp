/*
 * author:  Alexander Grill
 * file:    main.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "utils.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <asio.hpp>
#include <rang/rang.hpp>
#include <CLI11/CLI11.hpp>

#include <iostream>

using namespace std;
using namespace asio::ip;

int main(){
    unsigned short port = 1113;
    tcp::endpoint ep{tcp::v4(), port};
    asio::io_context cox;
    tcp::acceptor ap{cox, ep};

    std::map<std::string, int> test1;
    while(true){
        ap.listen();
        cout << "Server is listening" << endl;
        try{
            tcp::iostream strm{ap.accept()};
            string data = "";
            strm >> data;
            map<string, int>* mapclient = ConvertStringtoMap(data);
            
        }
        catch(...){
            cerr << "Error" << endl;
        }
    }
}
