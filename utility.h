#include <iostream>
#include <vector>
#define v6Groups 8
using namespace std;

typedef struct NetRange {
    unsigned long addr = 0;
    unsigned char mask = 0;
} netrange;

typedef struct Ipv6 {
    unsigned long long addr1 = 0;
    unsigned long long addr2 = 0;
} ipv6;

typedef struct NetRange6 {
    ipv6 addr;
    unsigned char mask = 0;
} netrange6;

void printNR4(netrange);

void printNR6(netrange6);

vector<string> split(string, string);

unsigned long v42ul(string);

netrange v4sub2nr(string);

ipv6 v62ipv6(string);

netrange6 v6sub2nr6(string);
