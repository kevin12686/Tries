#include "utility.h"
#include "string.h"

void printNR4(netrange nr) {
    unsigned char ip[4];
    unsigned long mask;
    for (int i = 0, shift = 24; i < 4; i++, shift -= 8) {
        mask = 0xff;
        mask <<= shift;
        ip[i] = (unsigned char)((nr.addr & mask) >> shift);
    }
    printf("%u.%u.%u.%u/%u\n", ip[0], ip[1], ip[2], ip[3], nr.mask);
}

void printNR6(netrange6 nr) {
    unsigned long ip[8];
    unsigned long long mask;
    int i = 0;
    for (unsigned char shift = 48; i < 4; i++, shift -= 16) {
        mask = 0xffff;
        mask <<= shift;
        ip[i] = (unsigned long)((nr.addr.addr1 & mask) >> shift);
    }
    for (unsigned char shift = 48; i < 8; i++, shift -= 16) {
        mask = 0xffff;
        mask <<= shift;
        ip[i] = (unsigned long)((nr.addr.addr2 & mask) >> shift);
    }
    printf("%x:%x:%x:%x:%x:%x:%x:%x/%u\n", ip[0], ip[1], ip[2], ip[3], ip[4],
           ip[5], ip[6], ip[7], nr.mask);
}

vector<string> split(string str, const char &del) {
    vector<string> v;
    string temp = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == del) {
            v.push_back(temp);
            temp = "";
        } else {
            temp += str[i];
        }
    }
    v.push_back(temp);
    return v;
}

unsigned long v42ul(string adrr) {
    unsigned long ul = 0;
    vector<string> addr_s = split(adrr, '.');
    for (vector<string>::iterator i = addr_s.begin(); i != addr_s.end(); i++) {
        ul <<= 8;
        ul |= (unsigned long)atol(i->c_str());
    }
    return ul;
}

netrange v4sub2nr(string subnet) {
    netrange nr;
    vector<string> net_s = split(subnet, '/');
    nr.addr = v42ul(net_s[0]);
    nr.mask = (unsigned char)atoi(net_s[1].c_str());
    return nr;
}

ipv6 v62ipv6(string addr) {
    ipv6 v6;
    v6.addr1 = 0;
    v6.addr2 = 0;
    unsigned char shiftc = 0;

    vector<string> addr_s = split(addr, ':');

    unsigned char fix = v6Groups - addr_s.size();

    for (vector<string>::iterator it = addr_s.begin(); it != addr_s.end();
         it++) {
        if (it->length() == 0) {
            for (int i = 1 + fix; i > 0; i--) {
                if (shiftc++ < 4) {
                    v6.addr1 <<= 16;
                } else {
                    v6.addr2 <<= 16;
                }
            }
        } else {
            if (shiftc++ < 4) {
                v6.addr1 <<= 16;
                v6.addr1 |= strtoull(it->c_str(), NULL, 16);
            } else {
                v6.addr2 <<= 16;
                v6.addr2 |= strtoull(it->c_str(), NULL, 16);
            }
        }
    }
    return v6;
}

netrange6 v6sub2nr6(string subnet) {
    netrange6 nr;
    vector<string> net_s = split(subnet, '/');
    nr.addr = v62ipv6(net_s[0]);
    nr.mask = (unsigned char)atoi(net_s[1].c_str());
    return nr;
}