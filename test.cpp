#include "utility.h"
#include <iostream>

using namespace std;

int main() {
    string s = "23.219.156.0/24";
    netrange nr = v4sub2nr(s);
    printf("%d, %d\n", nr.addr, nr.mask);
    printNR4(nr);

    s = "2001:500:7967::/48";
    NetRange6 nr6 = v6sub2nr6(s);
    printf("%llu, %llu, %d\n", nr6.addr.addr1, nr6.addr.addr2, nr6.mask);
    printNR6(nr6);

    return 0;
}