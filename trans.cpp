#include <iostream>
#include <string>
#include <stack>
#include <regex>
#include <math.h>
#include <cstring>
#include "main.hpp"

string transToRPN(string s)
{
    while (1)
    {
        regex reg0("(.*)[s][q][r][t](.*)");
        bool ret0 = regex_match(s, reg0);
        if (ret0)
        {
            s = judge(s, 0);
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        regex reg1("(.*)\\^(.*)");
        bool ret1 = regex_match(s, reg1);
        if (ret1)
        {
            s = judge(s, 1);
        }
        else
        {
            break;
        }
    }

    string ans;
    stack<pair<char, int>> op_stack;
    stack<char> brackets;
    int n = s.length();
    for (int i = 0; i < n; i++)
    {
        if ((s[i] == '-' && i == 0) || (s[i] == '-' && s[i - 1] == '('))
        {
            s = s.insert(i, "0");
            n++;
        }
    }
    n = s.length();
    for (int i = 0; i < n; i++)
    {
        if ((s[i] >= '0' && s[i] <= '9') || (s[i] == '-' && s[i - 1] == 'e') || s[i] == '.' || s[i] == 'e')
        {
            while ((s[i] >= '0' && s[i] <= '9') || (s[i] == '-' && s[i - 1] == 'e') || s[i] == '.' || s[i] == 'e')
            {
                ans.push_back(s[i]);
                i++;
            }
            ans.push_back(' ');
            i--;
        }
        else if (s[i] == '+' || s[i] == '-')
        {
            int current_priority = brackets.size() * 10 + 1;
            pair<char, int> p({s[i], current_priority});
            if (op_stack.empty())
            {
                op_stack.push(p);
            }
            else
            {
                pair<char, int> last_node = op_stack.top();
                if (current_priority > last_node.second)
                {
                    op_stack.push(p);
                }
                else
                {
                    while (!op_stack.empty() && current_priority <= op_stack.top().second)
                    {
                        ans.push_back(op_stack.top().first);
                        ans.push_back(' ');
                        op_stack.pop();
                    }
                    op_stack.push(p);
                }
            }
        }
        else if (s[i] == '*' || s[i] == '/')
        {
            int current_priority = brackets.size() * 10 + 2;
            pair<char, int> p({s[i], current_priority});
            if (op_stack.empty())
            {
                op_stack.push(p);
            }
            else
            {
                pair<char, int> last_node = op_stack.top();
                if (current_priority > last_node.second)
                {
                    op_stack.push(p);
                }
                else
                {
                    while (!op_stack.empty() && current_priority <= op_stack.top().second)
                    {
                        ans.push_back(op_stack.top().first);
                        ans.push_back(' ');
                        op_stack.pop();
                    }
                    op_stack.push(p);
                }
            }
        }
        else if (s[i] == '(')
        {
            brackets.push(s[i]);
        }
        else if (s[i] == ')')
        {
            brackets.pop();
        }
    }

    while (!op_stack.empty())
    {
        pair<char, int> p = op_stack.top();
        ans.push_back(p.first);
        ans.push_back(' ');
        op_stack.pop();
    }

    return ans;
}
