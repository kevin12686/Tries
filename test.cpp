#include "utility.h"
#include <iostream>

using namespace std;

int main() {
    string s = "23.219.156.0/24";
    netrange nr = v4sub2nr(s);
    printf("%d, %d\n", nr.addr, nr.mask);
    printNR4(nr);
    return 0;
}