#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <math.h>
#include "../main.hpp"

using namespace std;

string add(string aa, string bb, Database database[], int signtran)
{
    long long number1, number2, pointt, dis, sign1, sign2;
    string num1, num2;
    int aaa = stoi(aa);
    int bbb = stoi(bb);
    number1 = database[aaa].ae - database[aaa].point;
    number2 = database[bbb].ae - database[bbb].point;
    num1 = database[aaa].a1;
    num2 = database[bbb].a1;
    sign1 = database[aaa].sign;
    sign2 = database[bbb].sign;
    if (sign1 == 1 && sign2 == 0)
    {
        database[aaa].sign = 0;
        return min(bb, aa, database, 0);
    }
    if (sign1 == 1 && sign2 == 1)
    {
        database[aaa].sign = 0;
        database[bbb].sign = 0;
        return add(bb, aa, database, 1);
    }
    if (sign1 == 0 && sign2 == 1)
    {
        database[bbb].sign = 0;
        return min(aa, bb, database, 0);
    }

    if (number1 <= number2)
    {
        long long dif = number2 - number1;
        num2 += string(dif, '0');
        pointt = number1;
    }
    if (number1 > number2)
    {
        long long dif = number1 - number2;
        num1 += string(dif, '0');
        pointt = number2;
    }

    int a = num1.size() - 1, b = num2.size() - 1;
    int add = 0;
    string ss = "";
    while (a >= 0 || b >= 0 || add != 0)
    {
        if (a >= 0)
            add = add + num1[a--] - '0';
        if (b >= 0)
            add = add + num2[b--] - '0';
        ss = ss + to_string(add % 10);

        add = add / 10;
    }
    reverse(ss.begin(), ss.end());

    database[aaa].a1 = ss;
    database[aaa].ae = pointt;
    database[aaa].point = 0;
    database[aaa].sign = (signtran == 1) ? 1 : 0;
    // cout << database[aaa].sign << endl;
    string result = to_string(aaa);
    return result;
}