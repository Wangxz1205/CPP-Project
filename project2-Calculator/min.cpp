#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <math.h>
#include "main.hpp"

using namespace std;

string min(string aa, string bb, Database database[], int signtran)
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
        return add(aa, bb, database, 1);
    }
    if (sign1 == 1 && sign2 == 1)
    {
        database[aaa].sign = 0;
        database[bbb].sign = 0;
        return min(bb, aa, database, 1);
    }
    if (sign1 == 0 && sign2 == 1)
    {
        database[bbb].sign = 0;
        return add(aa, bb, database, 0);
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

    bool minus = false;
    if (cmp(num1, num2) < 0)
    {
        swap(num1, num2);
        minus = true;
    }

    int i = num1.size() - 1;
    int j = num2.size() - 1;
    int flag = 0;
    string ans = "";
    while (i >= 0 && j >= 0)
    {
        int tmp = 0;
        if (num1[i] - flag >= num2[j])
        {
            tmp = num1[i] - num2[j];
            tmp = tmp - flag;
            ans += to_string(tmp);
            flag = 0;
        }
        else
        {
            tmp = num1[i] - num2[j] + 10;
            tmp = tmp - flag;
            ans += to_string(tmp);
            flag = 1;
        }
        i--;
        j--;
    }
    // 处理较大数的剩余部分
    while (i >= 0)
    {
        if (flag == 0)
        {
            ans += num1[i];
        }
        else
        {
            int tmp = num1[i] - '0' - flag;
            if (tmp >= 0)
            {
                ans += to_string(tmp);
                flag = 0;
            }
            else
            {
                tmp = tmp + 10;
                ans += to_string(tmp);
                flag = 1;
            }
        }
        i--;
    }
    // 翻转
    reverse(ans.begin(), ans.end());
    //去除前导 0
    int k = 0;
    while (k < ans.size() && ans[k] == '0')
        k++;
    if (k == ans.size())
    {
        ans = "0";
    }
    else
        ans = ans.substr(k);
    database[aaa].a1 = ans;
    database[aaa].ae = pointt;
    database[aaa].point = 0;
    int w = minus ? 1 : 0;
    database[aaa].sign = (signtran == 1) ? 1 - w : w;
    string result = to_string(aaa);
    return result;
}