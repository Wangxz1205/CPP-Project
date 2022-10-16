#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include "main.hpp"

using namespace std;

string a2;

string data(string a, int n, Database database[])
{

    regex reg1("\\-?\\d\\.\\d+e\\-?\\d+");
    bool ret1 = regex_match(a, reg1);
    regex reg2("\\-?\\d+\\.\\d+");
    bool ret2 = regex_match(a, reg2);
    regex reg6("\\d+");
    bool ret6 = regex_match(a, reg6);
    if (ret1 == false && ret2 == false and ret6 == false)
    {
        return a;
    }
    database[n].sign = 0;
    if (a[0] == '-')
    {
        database[n].sign = 1;
        a.erase(a.begin());
    }
    regex reg3("(.*)\\e(.*)");
    smatch matchResult;
    bool ret3 = regex_match(a, matchResult, reg3);

    database[n].a1 = (ret3 == true) ? matchResult[1] : a;
    database[n].point = 0;
    a2 = (ret3 == true) ? matchResult[2] : string("0");
    database[n].ae = stoi(a2);
    regex reg4("(.*)\\.(.*)");
    smatch match;
    bool ret4 = regex_match(database[n].a1, match, reg4);
    if (ret4)
    {
        database[n].a1 = match[1];
        database[n].a1 += match[2];
        database[n].point = match[2].length();
       
    }
    return to_string(n);
}