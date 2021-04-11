/*
 * author:  Alexander Grill
 * file:    utils.cpp
 * project: mapreduce
 * date:    27.02.2021
*/

//includes
#include "utils.h"

#include <string.h>

#include <map>
#include <random>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <algorithm>

//namespaces
using namespace std;

//Funktionen Definitionen

/*
-Name: GetRandomNum
-Beschreibung: liefert eine zufällige Zahl von start bis end
-Input: int start, int end
-Output: int
*/
int GetRandomNum(int start, int end){
    random_device rd;
    mt19937 gen{rd()};
    uniform_int_distribution<> dis{start, end};
    int num = dis(gen);
    return num;
}

/*
-Name: IPIsValid
-Beschreibung: überprüft ob die mitübergebene IP Adresse valid is (korrekt is)
-Input: string ip
-Output: bool
*/
bool IPIsValid(string ip){
    int c{0};
    char *ip_input = strtok(&ip[0], ".");
    while (ip_input != NULL){
        string ipstring = ip_input;
        size_t pos;
        int num = stoi(ipstring, &pos);
        if (pos != ipstring.size()){
            return false;
        }
        if (num < 0 || num > 255){
            return false;
        }
        c += 1;
        ip_input = strtok(NULL, ".");
    }
    if (c < 4){
        return false;
    }
    return true;
}

/*
-Name: PORTIsValid
-Beschreibung: überprüft ob der mitübergebene PORT valid ist (korrekt ist)
-Input: string port
-Output: bool
*/
bool PORTIsValid(string port){
    size_t pos;
    int num = stoi(port, &pos);
    if (pos != port.size() || port.length() != 4){
        return false;
    }
    if (num < 0 || num > 65535){
        return false;
    }
    return true;
}

/*
-Name: Search
-Beschreibung: überprüft ob der mitübergebene string in der Map enthalten ist
-Input: string value, std::map<std::string, int>* dic
-Output: bool
*/
bool Search(string value, std::map<std::string, int>* dic){
    if (dic->find(value) == dic->end()){
        return false;
    }
    return true;
}

/*
-Name: Search
-Beschreibung: konvertiert die Map (dic Pointer auf das Objekt) zu einem string
-Input: std::map<std::string, int>* dic
-Output: string
*/
string ConvertMaptoString(std::map<std::string, int>* dic){
    string dicstring = "";
    for (map<string, int>::iterator i = dic->begin(); i != dic->end(); ++i){
        dicstring += i->first;
        dicstring += ",";
        dicstring += to_string(i->second);
        dicstring += ":";
    }
    return dicstring;
}

/*
-Name: Print
-Beschreibung: Printed die Map(dic Pointer auf das Objekt)
-Input: std::map<std::string, int>* dic
-Output: void
*/
void Print(std::map<std::string, int>* dic){
    int counter{0};
    cout << "Data Map" << endl;
    for (map<string, int>::iterator t = dic->begin(); t != dic->end(); ++t){
        cout << "Data " << t->first << " " << t->second << endl;
        counter += 1;
    }
    cout << "Elements: " << counter << endl;
}
