#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <math.h>
#include <queue>
#include <stack>
#include "../main.hpp"

using namespace std;

string cal(string s)
{
    queue<string> inorder;
    stringstream is(s);
    string out, c;
    int count = 1;
    Database database[20];
    while (is >> out)
    {
        out = data(out, count, database);
        inorder.push(out);
        count++;
    }
    stack<string> num;
    while (!inorder.empty())
    {
        string cur = inorder.front();
        inorder.pop();
        if (cur == "+")
        {
            string b = num.top();
            num.pop();
            string a = num.top();
            num.pop();
            c = add(a, b, database, 0);
            num.push(c);
        }
        else if (cur == "-")
        {
            string b = num.top();
            num.pop();
            string a = num.top();
            num.pop();
            c = min(a, b, database, 0);
            num.push(c);
        }
        else if (cur == "*")
        {
            string b = num.top();
            num.pop();
            string a = num.top();
            num.pop();
            c = mul(a, b, database);
            num.push(c);
        }
        else if (cur == "/")
        {
            string b = num.top();
            num.pop();
            string a = num.top();
            num.pop();
            c = div(a, b, database);
            num.push(c);
        }
        else
        {
            num.push(cur);
        }
    }
    int enumber = database[stoi(num.top())].ae - database[stoi(num.top())].point;
    string ans = database[stoi(num.top())].a1;
    if (enumber >= 0)
    {

        ans += string(enumber, '0');
    }

    else
    {
        long long dis = enumber + ans.size();
        regex reg10("[0]*$");
        regex reg11("[.]$");

        
        if (dis <= 0)
        {
            ans = "0." + string(-dis, '0') + ans;

            ans = regex_replace(ans, reg10, "");
        }
        else
        {

            ans.insert(dis, ".");

            ans = regex_replace(ans, reg10, "");

            ans = regex_replace(ans, reg11, "");
        }
    }
    // cout << ans << endl;
    if (database[stoi(num.top())].sign == 1)
    {
        ans.insert(0, "-");
    }
    return ans;
}
