#include <chrono>
#include <fstream>
#include <iostream>
using namespace std;

enum NType { PRIORITY, ORDINARY, EMPTY };

typedef struct Node {
    string prefix;
    NType _type;
    struct Node *left;
    struct Node *right;
} NODE;

NODE *create_node() {
    NODE *ptr = (NODE *)calloc(1, sizeof(NODE));
    ptr->prefix = "NULL";
    ptr->_type = NType::EMPTY;
    ptr->left = NULL;
    ptr->right = NULL;
    return ptr;
}

void build_node(NODE *root, string prefix, int level) {
    if (root->_type == NType::EMPTY) {
        root->prefix = prefix;
        if (prefix.length() > level)
            root->_type = NType::PRIORITY;
        else
            root->_type = NType::ORDINARY;
        return;
    } else {
        if (prefix.length() == level && root->_type == NType::PRIORITY) {
            string temp_prefix = root->prefix;
            root->prefix = prefix;
            prefix = temp_prefix;
            root->_type = NType::ORDINARY;
        } else if (prefix.length() > root->prefix.length() &&
                   prefix.substr(0, root->prefix.length()) == root->prefix &&
                   root->_type == NType::PRIORITY) {
            string temp_prefix = root->prefix;
            root->prefix = prefix;
            prefix = temp_prefix;
        }

        NODE *next = NULL;
        if (prefix[level++] == '0') {
            if (root->left == NULL)
                root->left = create_node();
            next = root->left;
        } else {
            if (root->right == NULL)
                root->right = create_node();
            next = root->right;
        }
        build_node(next, prefix, level);
    }
}

string search(NODE *root, string addr) {
    string BMP = "*";
    NODE *ptr = root;
    int level = 0;
    do {
        if (addr.substr(0, ptr->prefix.length()) == ptr->prefix) {
            BMP = ptr->prefix;
            if (ptr->_type == NType::PRIORITY)
                break;
        }
        if (addr[level++] == '0')
            ptr = ptr->left;
        else
            ptr = ptr->right;
    } while (ptr != NULL);
    return BMP;
}

void free_root(NODE *root) {
    if (root->left != NULL)
        free_root(root->left);
    if (root->right != NULL)
        free_root(root->right);
    free(root);
}

int main() {
    cout << "Priority Tries" << endl;
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
            build_node(root, buffer, 0);
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
            build_node(root, buffer, 0);
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