#include <chrono>
#include <fstream>
#include <iostream>
using namespace std;

typedef struct Node {
    string prefix;
    struct Node *left;
    struct Node *right;
} NODE;

// Create a node instance
NODE *create_node() {
    NODE *n = (NODE *)calloc(1, sizeof(NODE));
    n->prefix = "NULL";
    n->left = NULL;
    n->right = NULL;
    return n;
}

// Using to construct a binery tries with prefixes
void build_node(NODE *root, string prefix) {
    NODE *ptr = root;
    for (int i = 0; i < prefix.length(); i++) {
        if (prefix[i] == '0') {
            if (ptr->left == NULL) {
                ptr->left = create_node();
            }
            ptr = ptr->left;
        } else {
            if (ptr->right == NULL) {
                ptr->right = create_node();
            }
            ptr = ptr->right;
        }
    }
    ptr->prefix = prefix;
}

string search(NODE *root, string addr) {
    string BMP = "*";
    NODE *ptr = root;
    for (int i = 0; i < addr.length(); i++) {
        if (addr[i] == '0')
            ptr = ptr->left;
        else
            ptr = ptr->right;
        if (ptr == NULL ||
            ptr->prefix != "NULL" &&
                addr.substr(0, ptr->prefix.length()) != ptr->prefix)
            break;
        if (ptr->prefix != "NULL")
            BMP = ptr->prefix;
    }
    return BMP;
}

// Free memory
void free_root(NODE *root) {
    if (root->left != NULL)
        free_root(root->left);
    if (root->right != NULL)
        free_root(root->right);
    free(root);
}

int main() {
    cout << "Binary Tries" << endl;
    const string b_filename = "prefix_v4_build";
    const string u_filename = "prefix_v4_update";
    const string s_filename = "testing_bin";
    NODE *root = create_node();
    string buffer;

    // Build
    ifstream b_file(b_filename);
    // Failed to open file
    if (!b_file.is_open())
        return 1;

    // Constructing binary tries
    while (getline(b_file, buffer)) {
        if (buffer.length() > 0) {
            build_node(root, buffer);
        }
    }
    b_file.close();

    // Update
    ifstream u_file(u_filename);
    // Failed to open file
    if (!u_file.is_open())
        return 1;
    // Constructing binary tries
    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();
    while (getline(u_file, buffer)) {
        if (buffer.length() > 0) {
            build_node(root, buffer);
        }
    }
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    u_file.close();
    std::cout << "Update Time = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                       begin)
                     .count()
              << "[µs]" << std::endl;

    // Search
    ifstream s_file(s_filename);
    // Failed to open file
    if (!s_file.is_open())
        return 1;
    // Constructing binary tries
    begin = std::chrono::steady_clock::now();
    while (getline(s_file, buffer)) {
        if (buffer.length() > 0) {
            search(root, buffer);
        }
    }
    end = std::chrono::steady_clock::now();
    s_file.close();
    std::cout << "Search Time = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                       begin)
                     .count()
              << "[µs]" << std::endl;

    // Free Memory
    free_root(root);

    return 0;
}