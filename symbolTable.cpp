#include <iostream>
#include <string>
using namespace std;

struct Node {
    string key;
    string value;
    Node* left;
    Node* right;

    Node(string k, string v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

class SymbolTable {
private:
    Node* root;

    Node* insert(Node* node, string key, string value) {
        if (node == nullptr)
            return new Node(key, value);

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else
            node->value = value;

        return node;
    }

    string search(Node* node, string key) {
        if (node == nullptr)
            return "Key not found";
        if (key < node->key)
            return search(node->left, key);
        else if (key > node->key)
            return search(node->right, key);
        else return node->value;
    }

public:
    SymbolTable(){
        root = nullptr;
    }

    void insert(string key, string value) {
        if(search(root, key) == "Key not found")
            root = insert(root, key, value);
    }

    string search(string key) {
        return search(root, key);
    }
};

int main() {

    SymbolTable identifierTable;
    SymbolTable constantTable;

    identifierTable.insert("id1", "v1");
    identifierTable.insert("id2", "v2");
    constantTable.insert("c1", "v3");

    cout << "Value of identifier1: " << identifierTable.search("id1") << '\n';
    cout << "Value of constant1: " << constantTable.search("c1") << '\n';
    cout << "Value of identifier3: " << identifierTable.search("id3") << '\n';

    return 0;
}

