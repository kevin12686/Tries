#include "utility.h"
#include "string.h"

void printNR4(netrange nr) {
    unsigned char ip[4];
    unsigned long long mask;
    for (int i = 0, shift = 24; i < 4; i++, shift -= 8) {
        mask = 0xff;
        mask <<= shift;
        ip[i] = (unsigned char)((nr.addr & mask) >> shift);
    }
    printf("%u.%u.%u.%u/%u\n", ip[0], ip[1], ip[2], ip[3], nr.mask);
}

vector<string> split(string str, string del) {
    vector<string> v;
    char *buf;
    char *ptr = strtok_s(const_cast<char *>(str.c_str()), del.c_str(), &buf);
    while (ptr) {
        v.push_back(string(ptr));
        ptr = strtok_s(NULL, ".", &buf);
    }
    return v;
}

unsigned long long v42ull(string adrr) {
    vector<string> addr_s = split(adrr, ".");
    unsigned long long ull = 0;
    for (vector<string>::iterator i = addr_s.begin(); i != addr_s.end(); i++) {
        ull <<= 8;
        ull |= (unsigned long long)atoll(i->c_str());
    }
    return ull;
}

netrange v4sub2nr(string subnet) {
    netrange nr;
    vector<string> net_s = split(subnet, "/");
    nr.addr = v42ull(net_s[0]);
    nr.mask = (unsigned char)atoi(net_s[1].c_str());
    return nr;
}