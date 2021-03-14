/*
 * author:  Alexander Grill
 * file:    slaveserver.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

#include "slaveserver.h"
#include "utils.h"

#include <iostream>

using namespace std;

Slaveserver* Slaveserver::GetSlaveServer(std::string pr){
    bool povalid;
    Slaveserver *ssl;

    povalid = PORTIsValid(pr);
    /*cout << fg::green << flush;
    spdlog::get("client_logger")->info("IP Adresse is valid " + to_string(ipvalid));
    spdlog::get("file_logger")->info("IP Adresse is valid " + to_string(ipvalid));
    cout << fg::green << flush;
    spdlog::get("client_logger")->info("PORT is valid " + to_string(povalid));
    spdlog::get("file_logger")->info("PORT is valid " + to_string(povalid));*/

    if (povalid == false){
        std::cerr << "IP Adress or Port is invalid!" << std::endl;
        ssl = nullptr;
    }
    else{
        ssl = new Slaveserver(pr);
    }
    return ssl;
}