#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <math.h>
#include "../main.hpp"

using namespace std;
int cmp(string a, string b)
{
    if (a.find_first_not_of('0') == string::npos)
        a = "0";
    else
        a.substr(a.find_first_not_of('0'));
    if (b.find_first_not_of('0') == string::npos)
        b = "0";
    else
        b.substr(b.find_first_not_of('0'));

    if (a.size() > b.size())
        return 1;
    else if (a.size() < b.size())
        return -1;
    else //长度相等
    {
        if (a < b)
            return -1;
        else if (a > b)
            return 1;
        else
            return 0;
    }
}