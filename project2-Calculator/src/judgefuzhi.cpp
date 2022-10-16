#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include "../main.hpp"

using namespace std;

string &replace_all(string &src, const string &old_value, const string &new_value)
{

    for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
    {
        if ((pos = src.find(old_value, pos)) != string::npos)
        {
            src.replace(pos, old_value.length(), new_value);
        }
        else
            break;
    }
    return src;
}

string judgefuzhi(string a, int n, Fuzhi fuzhi[])
{
    regex reg0("(.*)\\=(.*)");
    smatch match0;
    bool ret0 = regex_match(a, match0, reg0);

    if (ret0)
    {
        fuzhi[n].symbol = match0[1];
        string take = match0[2];
        string numbertake = "(" + take + ")";
        fuzhi[n].number = numbertake;

        return "w";
    }

    if (!ret0)
    {
        for (int i = 1; i < n; i++)
        {
            a = replace_all(a, fuzhi[i].symbol, fuzhi[i].number);
        }
    }
    return a;
}
