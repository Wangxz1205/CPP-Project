#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <math.h>
#include "main.hpp"

using namespace std;

string mul(string aa, string bb, Database database[])
{
    long long point, enumber;
    string mulans;
    int aaa = stoi(aa);
    int bbb = stoi(bb);
    point = database[aaa].point + database[bbb].point;
    enumber = database[aaa].ae + database[bbb].ae;

    int a = database[aaa].a1.size(), b = database[bbb].a1.size();
    vector<int> mul(a + b);
    for (int i = a - 1; i >= 0; i--)
    {
        for (int j = b - 1; j >= 0; j--)
        {
            int answer = int(database[aaa].a1[i] - '0') * int(database[bbb].a1[j] - '0');
            mul[i + j + 1] += answer;
        }
    }
    for (int i = a + b - 1; i >= 1; i--)
    {
        mul[i - 1] += mul[i] / 10;
        mul[i] = mul[i] % 10;
    }
    for (int i = 0; i < a + b; i++)
    {
        mulans += to_string(mul[i]);
    }
    if (mulans[0] == '0'&& mulans != "0")
    {
        mulans.erase(mulans.begin());
    }
    database[aaa].sign = ((database[aaa].sign + database[bbb].sign) == 1) ? 1 : 0;
    enumber = enumber - point;
    database[aaa].a1 = mulans;
    database[aaa].ae = enumber;
    database[aaa].point = 0;

    return to_string(aaa);
}