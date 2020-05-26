#include "time.h"
#include "utility.h"
#include <fstream>
#include <iostream>
#define IPbits 32
using namespace std;

enum NType { PRIORITY, ORDINARY, EMPTY };

typedef struct Node {
    netrange net;
    NType _type = NType::EMPTY;
    struct Node *left = NULL;
    struct Node *right = NULL;
} node;

node *create_node() {
    node *ptr = (node *)calloc(1, sizeof(node));
    ptr->_type = NType::EMPTY;
    ptr->left = NULL;
    ptr->right = NULL;
    return ptr;
}

void build_node(node *root, netrange net, int level) {
    if (root->_type == NType::EMPTY) {
        root->net = net;
        if (net.mask > level)
            root->_type = NType::PRIORITY;
        else
            root->_type = NType::ORDINARY;
        return;
    } else {
        unsigned char shift = IPbits - root->net.mask;
        if (net.mask == level && root->_type == NType::PRIORITY) {
            netrange temp_net = root->net;
            root->net = net;
            net = temp_net;
            root->_type = NType::ORDINARY;
        } else if (net.mask > root->net.mask &&
                   net.addr >> shift == root->net.addr >> shift &&
                   root->_type == NType::PRIORITY) {
            netrange temp_net = root->net;
            root->net = net;
            net = temp_net;
        }
        node *next = NULL;
        unsigned long long mask = 1;
        unsigned long long sbit = 0;
        mask <<= (IPbits - (++level) - 1);
        sbit = net.addr & mask;

        if (sbit == 0) {
            if (root->left == NULL)
                root->left = create_node();
            next = root->left;
        } else {
            if (root->right == NULL)
                root->right = create_node();
            next = root->right;
        }
        build_node(next, net, level);
    }
}

netrange search(node *root, unsigned long long addr) {
    netrange BMP;
    BMP.addr = 0;
    BMP.mask = 0;
    node *ptr = root;
    unsigned char level = 0;
    unsigned char shift = 0;
    unsigned long long mask = 1;
    unsigned long long sbit = 0;
    do {
        shift = IPbits - ptr->net.mask;
        if (addr >> shift == ptr->net.addr >> shift) {
            BMP = ptr->net;
            if (ptr->_type == NType::PRIORITY)
                break;
        }
        mask = 1;
        mask <<= (IPbits - (++level) - 1);
        sbit = addr & mask;
        if (sbit == 0)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    } while (ptr != NULL);
    return BMP;
}

void free_root(node *root) {
    if (root->left != NULL)
        free_root(root->left);
    if (root->right != NULL)
        free_root(root->right);
    free(root);
}

int main() {
    cout << "Priority Tries" << endl;
    const string b_filename = "./data/ipv4_build.txt";
    const string u_filename = "./data/ipv4_update.txt";
    const string s_filename = "./data/ipv4_search_500000.txt";

    node *root = create_node();
    string buffer;
    clock_t begin, end;

    // Build
    ifstream b_file(b_filename);
    // Failed to open file
    if (!b_file.is_open()) {
        cout << "Failed to open file." << endl;
        return 1;
    }
    // Constructing priority tries
    while (getline(b_file, buffer)) {
        if (buffer.length() > 0) {
            build_node(root, v4sub2nr(buffer), 0);
        }
    }
    b_file.close();

    // Update
    ifstream u_file(u_filename);
    // Failed to open file
    if (!u_file.is_open()) {
        cout << "Failed to open file." << endl;
        return 1;
    }
    // Updating priority tries
    begin = clock();
    while (getline(u_file, buffer)) {
        if (buffer.length() > 0) {
            build_node(root, v4sub2nr(buffer), 0);
        }
    }
    end = clock();
    u_file.close();
    cout << "Update: " << end - begin << " clocks" << endl;

    // Search
    ifstream s_file(s_filename);
    // Failed to open file
    if (!s_file.is_open()) {
        cout << "Failed to open file." << endl;
        return 1;
    }
    // Searching priority tries
    begin = clock();
    while (getline(s_file, buffer)) {
        if (buffer.length() > 0) {
            search(root, v42ull(buffer));
        }
    }
    end = clock();
    s_file.close();
    cout << "Search: " << end - begin << " clocks" << endl;

    // Free Memory
    free_root(root);

    return 0;
}