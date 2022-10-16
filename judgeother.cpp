#include <iostream>
#include <string>
#include <stack>
#include <regex>
#include <math.h>
#include <cstring>
#include "main.hpp"

using namespace std;

string judge(string s, int n)
{
    if (n == 0)
    {
        regex reg0("(.*)[s][q][r][t][(](.*)[)](.*)");
        smatch match0;
        bool ret0 = regex_match(s, match0, reg0);
        if (ret0)
        {
            string a = match0[1];
            string b = match0[2];
            string c = match0[3];
            int count;
            int clong = a.length();
            string input;
            count = b.length();

            input = transToRPN(b);
            input = cal(input);
            if(stod(input)<0){
                cout << "Invalid input" << endl;
                exit(-1);
            }
            double t = stod(input);
            t = sqrt(t);
            s = s.replace(clong, s.size() - clong, "");
            s += to_string(t);
            s += c;
        }
    }

    if (n == 1)
    {
        regex reg1("(.*)[(](.*)[)][^](.*)[+ - * / )]?(.*)");
        smatch match1;
        bool ret1 = regex_match(s, match1, reg1);
        if (ret1)
        {
            string a = match1[1];
            string b = match1[2];
            string c = match1[3];
            string d = match1[4];
            int count;
            int clong = a.length();
            string input;
            count = b.length();
            input = transToRPN(b);
            input = cal(input);
            double t = stod(input);
            int pownumber = stoi(c);
            t = pow(t, pownumber);
            s = s.replace(clong, s.size() - clong, "");
            s += to_string(t);
            s += d;
            
        }
    }

    return s;
}