#include <bitset>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

using namespace std;

typedef struct Subnet {
    bitset<32> ip;
    bitset<32> mask;
    int length;
} subnet;

typedef struct Node {
    struct Node *left;
    struct Node *right;
    subnet *prefix;
} node;

// Create a node instance
node *create_node(node *left, node *right, subnet *prefix) {
    node *n = (node *)calloc(1, sizeof(node));
    n->left = left;
    n->right = right;
    n->prefix = prefix;
    return n;
}

// Free memory
void free_root(node *root) {
    if (root->left != NULL)
        free_root(root->left);
    if (root->right != NULL)
        free_root(root->right);
    free(root);
}

// Using to construct a binery tries with prefixes
void pushing(node *root, subnet *prefix) {
    node *ptr = root;
    bitset<32> net = prefix->ip & prefix->mask;
    for (int i = 31; i >= 32 - prefix->length; i--) {
        if (net.test(i)) {
            if (ptr->right == NULL) {
                ptr->right = create_node(NULL, NULL, NULL);
            }
            ptr = ptr->right;
        } else {
            if (ptr->left == NULL) {
                ptr->left = create_node(NULL, NULL, NULL);
            }
            ptr = ptr->left;
        }
    }
    ptr->prefix = prefix;
}

// Extracting the prefix data from raw input
string extract_prefix(string data) {
    istringstream stream(data);
    string buf;
    for (int i = 0; i < 6; i++) {
        getline(stream, buf, '|');
    }
    return buf;
}

// Convert string of ipv4 into subnet data type.
subnet ipv4_to_subnet(string ipv4) {
    int a, b, c, d, e;
    sscanf(ipv4.c_str(), "%d.%d.%d.%d/%d", &a, &b, &c, &d, &e);
    bitset<32> mask(0);
    for (int i = 31; i >= 32 - e; i--) {
        mask.set(i, 1);
    }
    bitset<32> ba(a);
    bitset<32> bb(b);
    bitset<32> bc(c);
    bitset<32> bd(d);
    bitset<32> ip(0);
    ip = ba << 24 | bb << 16 | bc << 8 | bd;

    subnet net;
    net.ip = ip;
    net.mask = mask;
    net.length = e;
    return net;
}

int main() {
    set<string> prefixes_set;
    string buffer;

    ifstream file("rip");

    // Failed to open file
    if (!file.is_open())
        return 1;

    // Extracting prefix data
    while (getline(file, buffer)) {
        buffer = extract_prefix(buffer);

        // IPv4 only
        if (buffer.find(':') == string::npos) {
            prefixes_set.insert(buffer);
        }
    }
    file.close();

    // Construct subnet array
    int n = prefixes_set.size();
    subnet nets[n];
    int i = 0;
    set<string>::iterator it = prefixes_set.begin();
    for (int i = 0; i < n, it != prefixes_set.end(); i++, it++) {
        nets[i] = ipv4_to_subnet(*it);
    }

    // Create a root node of binary tries
    node *root = create_node(NULL, NULL, NULL);

    // Constructing binary tries
    for (int i = 0; i < n; i++) {
        pushing(root, &nets[i]);
    }

    // Free Memory
    free_root(root);

    return 0;
}