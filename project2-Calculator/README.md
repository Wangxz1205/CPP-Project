# CS205 C/ C++ Programming - Project2

<font size=3>**Name:** 王习之（Wang Xizhi）

**SID:** 11911818</font>

## Part1 - Analysis
---
因为在project1中我已经完成了高精度且可完成大数运算的乘法器，在此基础上，我在第二个project中完成了更多功能的计算器，可以实现普通的运算以及赋值运算。为了运算时间更快，我设置了运算数字总共可以到20个，同时为了实现能够科学计数法的数字运算，我建立了一个structure结构体来储存每一个数字的详细信息，包括小数点的位置，数字的正负等等。
```c++
struct Database
{
    long long point;
    string a1;
    int sign, ae;
};
```
整个改进的运算器我预计实现的算法有加法，减法，乘法，除法，开方以及平方多次方的计算，同时也实现了赋值计算，数字的范围从高精度的整数到小数，再到科学计数法都可以实现。

在进行普通运算时，会出现乘除法的优先级，以及括号的更高级，所以我考虑使用了逆波兰表达式。在平时我们更习惯于使用中缀表达式，例如`(2+3)*5`这样的，但是这在计算机语言中是很麻烦的，所以我考虑将其转为后缀表达式，也是逆波兰表达式的一种，可以转为`2 3 + 5 *`这样就可以让计算机明白我们表达式的计算顺序。而要转化为逆波兰表达式，我则需要用到栈`stack`。栈遵循先进后出的原则，所以当输入数字的时候，数字保留在计算的栈中，而判断为运算符号时，就可以进行相应的运算。

同时，我实现了更多功能，有开方以及指数运算，这是我们的表达式中会出现`sqrt(2)`或者`(3)^3`这样的表示式子。和第一次project相同，我也使用了正则表达式，去判断，提取了相应的字符，同时先运算了括号中的内容，将结果替换原字符串相应的内容，再转化为逆波兰表达式。

正则表示式则是在我的整个代码中都多次使用，判断数字是否为规范的数字，分解数字的各个信息，判断是否为开放或者指数运算都是必要的。

在判断是赋值操作时，为了整个操作系统更加的直观和智能，我在开头可以让用户选择当前的模式，如下，
```
选择你的计算模式(a.普通 b.赋值）： 
输入你的表达式：
```
这样当出现例如`x=2`的赋值操作时，我也新建了一个结构体来储存相应符号对应的数字
```
struct Fuzhi
{
    string symbol, number;
};
```
当出现运算表达式的时候，我会将符号转为相应的数字，再进行逆波兰表达式的转换。

## Part2 - Code
---
```c++
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
```

## Part3 - Result & Verification
---
Test case #1: Integer multiplication

![Alternative text](1.png)

Test case #2: Floating-point number multiplication

![Alternative text](2.png)
![Alternative text](3.png)

Test case #3: Not number

![Alternative text](4.png)

Test case #4: Big integer multiplication

![Alternative text](5.png)

Test case #5: Big  floating-point number multiplication

![Alternative text](6.png)

## Part4 - Difficulties & Solutions
---
First, how to deal with String data sets. Later, I searched the Internet for the knowledge related to regular expressions, and learned how to match, extract, segment and other important functions of data, which is ` #include <regex>`.

Then after I split the string, I did the multiplication of the string traversal method, and the effect of the decimal point on me was also quite big. I recorded the position of the decimal point, and finally determined whether the decimal point was in the middle, before, or after the end of the result, the output of different processing.

The most useful thing I've learned in this whole process is the regular expression. It's a very convenient way to know if a string is what we want, and it's also a very useful way to extract only what we want.

 

 
