#include <iostream>
#include <string>
#include <regex>

using namespace std;
string a1, a2, b1, b2, mulans;
long long point1, point2, point;
int sign, ae, be, enumber;

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        string a = argv[i];
        regex reg1("\\-?\\d\\.\\d*e\\-?\\d+");
        bool ret1 = regex_match(a, reg1);
        regex reg2("\\-?\\d+\\.?\\d+");
        bool ret2 = regex_match(a, reg2);
        regex reg6("\\d+"); 
        bool ret6 = regex_match(a, reg6);
        if (ret1 == false && ret2 == false and ret6 == false)
        {
            printf("The input cannot be interpret as numbers!\n");
            return 0;
        }
        if (a[0] == '-')
        {
            sign++;
            a.erase(a.begin());
        }
        regex reg3("(.*)\\e(.*)");
        smatch matchResult;
        bool ret3 = regex_match(a, matchResult, reg3);
        if (i == 1)
        {
            a1 = (ret3 == true) ? matchResult[1] : a;
            a2 = (ret3 == true) ? matchResult[2] : string("0");
            ae = stoi(a2);
            regex reg4("(.*)\\.(.*)");
            smatch match;
            bool ret4 = regex_match(a1, match, reg4);
            if (ret4)
            {
                a1 = match[1];
                a1 += match[2];
                point1 = match[2].length();
            }
        }
        else if (i == 2)
        {
            b1 = (ret3 == true) ? matchResult[1] : a;
            b2 = (ret3 == true) ? matchResult[2] : string("0");
            be = stoi(b2);
            regex reg4("(.*)\\.(.*)");
            smatch match;
            bool ret4 = regex_match(b1, match, reg4);
            if (ret4)
            {
                b1 = match[1];
                b1 += match[2];
                point2 = match[2].length();
            }
        }
    }
    point = point1 + point2;
    enumber = ae + be;
    int a = a1.size(), b = b1.size();
    vector<int> mul(a + b);
    for (int i = a - 1; i >= 0; i--)
    {
        for (int j = b - 1; j >= 0; j--)
        {
            int answer = int(a1[i] - '0') * int(b1[j] - '0');
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
    if (mulans[0] == '0')
    {
        mulans.erase(mulans.begin());
    }
    string Sign = (sign == 1) ? "-" : "";
    enumber = enumber - point;
    if (enumber >= 0)
    {
        mulans += string(enumber, '0');
    }
    else
    {
        long long dis = enumber + mulans.size();
        regex reg10("[0 .]*$");
        if (dis <= 0)
        {
            mulans = "0." + string(-dis, '0') + mulans; 
            mulans = regex_replace(mulans, reg10, "");
        }
        else
        {
            mulans.insert(dis, ".");
            mulans = regex_replace(mulans, reg10, "");
        }
    }
    cout << argv[1] << " * " << argv[2] << " = " << Sign << mulans << endl;
    return 0;
}
