#include <iostream>
#include <vector>
using namespace std;

typedef struct NetRange {
    unsigned long long addr = 0;
    unsigned char mask = 0;
} netrange;

void printNR4(netrange);

vector<string> split(string, string);

unsigned long long v42ull(string);

netrange v4sub2nr(string);
