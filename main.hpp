#include <string>
using namespace std;
struct Database
{
    long long point;
    string a1;
    int sign, ae;
};
struct Fuzhi
{
    string symbol, number;
};
string transToRPN(string s);
string data(string a, int n, Database database[]);
string cal(string s);
string mul(string aa, string bb, Database database[]);
string add(string aa, string bb, Database database[], int signtran);
string min(string aa, string bb, Database database[], int signtran);
string div(string aa, string bb, Database database[]);
int cmp(string a, string b);
string judgefuzhi(string a, int n, Fuzhi fuzhi[]);
string judge(string s,int n);