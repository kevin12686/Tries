#include "utility.h"
#include <fstream>
#include <iostream>
#include <time.h>
#define IPbits 32

using namespace std;

typedef struct Node {
    netrange net;
    bool empty = true;
    struct Node *left = NULL;
    struct Node *right = NULL;
} node;

node *create_node() {
    node *n = (node *)calloc(1, sizeof(node));
    n->empty = true;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void free_nodes(node *root) {
    if (root->left)
        free_nodes(root->left);
    if (root->right)
        free_nodes(root->right);
    free(root);
}

// Using to construct a binery tries with prefixes
void build_node(node *root, netrange net) {
    unsigned long long mask = 1;
    unsigned long long sbit = 0;
    node *ptr = root;
    for (int i = IPbits - 1; i >= IPbits - net.mask; i--) {
        mask = 1;
        mask <<= i;
        sbit = net.addr & mask;
        if (sbit == 0) {
            if (!ptr->left) {
                ptr->left = create_node();
            }
            ptr = ptr->left;
        } else {
            if (!ptr->right) {
                ptr->right = create_node();
            }
            ptr = ptr->right;
        }
    }
    ptr->net = net;
    ptr->empty = false;
}

netrange search(node *root, unsigned long long addr) {
    unsigned long long mask = 1;
    unsigned long long sbit = 0;
    unsigned char shift = 0;
    node *ptr = root;
    netrange BMP;

    for (int i = IPbits - 1; i >= 0; i--) {
        mask = 1;
        mask <<= i;
        sbit = addr & mask;

        if (sbit == 0) {
            if (!ptr->left)
                break;
            ptr = ptr->left;
        } else {
            if (!ptr->right)
                break;
            ptr = ptr->right;
        }

        if (!ptr->empty) {
            shift = IPbits - ptr->net.mask;
            if (addr >> shift == ptr->net.addr >> shift) {
                BMP = ptr->net;
            } else {
                break;
            }
        }
    }
    return BMP;
}

int main() {
    cout << "Binary Tries" << endl;
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
    // Constructing binary tries
    while (getline(b_file, buffer)) {
        if (buffer.length() > 0) {
            build_node(root, v4sub2nr(buffer));
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
    // Updating binary tries
    begin = clock();
    while (getline(u_file, buffer)) {
        if (buffer.length() > 0) {
            build_node(root, v4sub2nr(buffer));
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
    // Searching binary tries
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
    free_nodes(root);

    return 0;
}