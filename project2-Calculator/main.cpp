#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <math.h>
#include "main.hpp"

using namespace std;
string asd;

int main()
{
    cout << "选择你的计算模式(a.普通 b.赋值）： ";
    string choose;
    getline(cin, choose);
    cout << "输入你的表达式： " << endl;
    if (choose == "a")
    {
        string input;
        getline(cin, input);
        input = transToRPN(input);

        asd = cal(input);
    }
    if (choose == "b")
    {
        string input;
        Fuzhi fuzhi[20];
        int count = 1;
        for (int i = 0; i < count; i++)
        {
            getline(cin, input);
            input = judgefuzhi(input, count, fuzhi);
            if (input != "w")
            {
                input = transToRPN(input);
                asd = cal(input);

                break;
            }
            count++;
        }
    }
    cout << "答案是： " << endl;
    cout << asd << endl;

    return 0;
}
