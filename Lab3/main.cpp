#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
        root = insert(root, key, value);
    }

    string search(string key) {
        return search(root, key);
    }

    void print_content(){
        ofstream stOutFile("ST.out");
        while(root != nullptr)
            stOutFile << root->key << '\n', root = root->left;
    }
};

SymbolTable symbolTable;

void insertIntoSymbolTable(string token, string category) {
    symbolTable.insert(token, category);
}

int main() {
    ifstream tokenFile("token.in");
    ofstream pifout("PIF.out");

    string token;
    while (tokenFile >> token) {
        insertIntoSymbolTable(token, "Token");
    }

    vector<string> keywords = {"function", "as", "integer", "vector", "reader", "writer", "while", "if", "else", "for"};
    vector<string> operators = {"+", "-", "/", "%", "*", ">", "<", ">=", "<=", "!=", "==", "(", ")", "{", "}", ",", "//", "\\\\", ".", "<-", "than", "in"};

    string validIdentifierChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string validNumberChars = "0123456789";

    // Code Input File
    ifstream inputFile("p3");

    string line;
    int lineNumber = 1;

    while (getline(inputFile, line)) {
        string token;
        string tokenCategory;

        for (int i = 0; i < line.length(); i++) {
            char c = line[i];

            if (c == ' ' || c == '\t') {
                continue;
            } else if (c == '#') {
                if (i + 1 < line.length() ) {
                    break;
                }
            } else if (isalpha(c) || validIdentifierChars.find(c) != string::npos) {
                token = c;
                i++;
                while (i < line.length() && (isalnum(line[i]) || validIdentifierChars.find(line[i]) != string::npos)) {
                    token += line[i];
                    i++;
                }
                i--;

                if (find(keywords.begin(), keywords.end(), token) != keywords.end()) {
                    tokenCategory = "Keyword";
                } else {
                    tokenCategory = "Identifier";
                }
            } else if (isdigit(c)) {

                token = c;
                i++;
                while (i < line.length() && (isdigit(line[i]) || validNumberChars.find(line[i]) != string::npos)) {
                    token += line[i];
                    i++;
                }
                i--;

                tokenCategory = "Number";
            } else {

                token = c;
                i++;
                if (i < line.length()) {
                    token += line[i];
                }

                if (find(operators.begin(), operators.end(), token) != operators.end()) {
                    tokenCategory = "Operator";
                } else {
                    tokenCategory = "Symbol";
                }
            }

            if (!tokenCategory.empty()) {
                insertIntoSymbolTable(token, tokenCategory);
                pifout << token << " : " << tokenCategory << endl;
            }
        }

        lineNumber++;
    }

    symbolTable.print_content();

    cout << "lexically correct" << endl;

    return 0;
}
