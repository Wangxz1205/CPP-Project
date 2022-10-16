#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <math.h>
#include "../main.hpp"

using namespace std;

string substract(string a, string b)
{
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());

    for (int i = 0; i < b.size(); i++)
    {
        if (a[i] >= b[i])
        {
            a[i] = (a[i] - b[i] + '0');
        }
        else
        {
            int k = 1;
            while (a[i + k] == '0')
            {
                a[i + k] = '9';
                k++;
            }

            a[i + k] = a[i + k] - '1' + '0';

            a[i] = a[i] - '0' + 10 - (b[i] - '0') + '0';
        }
    }
    reverse(a.begin(), a.end());

    if (a.find_first_not_of('0') == string::npos)
    {
        return "0";
    }
    return a.substr(a.find_first_not_of('0'));
}

string div(string aa, string bb, Database database[])
{

    long long number1, number2, pointt;
    string divans = "";
    string num1, num2;
    int aaa = stoi(aa);
    int bbb = stoi(bb);
    number1 = database[aaa].ae - database[aaa].point;
    number2 = database[bbb].ae - database[bbb].point;
    num1 = database[aaa].a1;
    num2 = database[bbb].a1;

    if (num2 == "0")
    {
        cout << "Invalid input" << endl;
        exit(-1);
    }

    pointt = number1 - number2;

    int a = num1.size(), b = num2.size();
    long long pointadd = 0;
    if (a < b)
    {
        pointadd = b - a;
        a = b;
        num1 += string(pointadd, '0');
    }
    pointt = pointt - pointadd;

    int add = 0;
    int count = 0;
    if (a >= b)
    {
        int dif = a - b;
        add = dif;
        for (int i = 0; i < dif + 7; i++)
        {
            int t = 0;
            string take = num2;
            if (add == -1)
            {
                num1 += string(1, '0');
                add = 0;
                count++;
            }
            num2 += string(add, '0');

            while (cmp(num1, num2) >= 0)
            {
                // cout << num1 << endl;
                // cout << num2 << endl;
                num1 = substract(num1, num2);
                t++;
            }
            divans.append(to_string(t));
            num2 = take;
            add--;
        }
    }

    while (1)
    {
        if (divans[0] == '0')
        {
            divans.erase(divans.begin());
        }
        else
        {
            break;
        }
    }
    pointt = pointt - count;

    database[aaa].sign = ((database[aaa].sign + database[bbb].sign) == 1) ? 1 : 0;
    database[aaa].a1 = divans;
    database[aaa].ae = pointt;
    database[aaa].point = 0;

    string result = to_string(aaa);
    return result;
}