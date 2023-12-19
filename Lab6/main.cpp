#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using namespace std;

struct Production {
    string nonterminal;
    vector<string> symbols;
};

unordered_set<string> nonterminals;
unordered_set<string> terminals;
unordered_map<string, vector<Production>> productions;

unordered_map<string, unordered_set<string>> firstSets;
unordered_map<string, unordered_set<string>> followSets;

void readGrammar(const string& filename) {
    ifstream file(filename);

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string nonterminal;
        iss >> nonterminal;
        nonterminals.insert(nonterminal);

        string arrow;
        iss >> arrow;

        string symbol;
        vector<string> production;
        while (iss >> symbol) {
            if (symbol != "|") {
                production.push_back(symbol);
                terminals.insert(symbol);
            }
        }

        productions[nonterminal].push_back({nonterminal, production});
    }

    file.close();
}

void printNonTerminals() {
    cout << "NonTerminals : { ";
    for (const auto& element : nonterminals) {
        cout << element << " ";
    }
    cout << "}" << '\n';
}

void printTerminals(){
    cout << "Terminals : { ";
    for(const auto& element : terminals){
        if(nonterminals.find(element) == nonterminals.end())
            cout << element << " ";
    }
    cout << "}" << '\n';
}

void printProductions(const string& nonterminal) {
    if (productions.find(nonterminal) != productions.end()) {
        cout << "Productions for " << nonterminal << ": " << endl;
        for (const auto& prod : productions[nonterminal]) {
            cout << prod.nonterminal << " -> ";
            for (const auto& symbol : prod.symbols) {
                cout << symbol << " ";
            }
            cout << '\n';
        }
    } else {
        cout << "No productions found for " << nonterminal << endl;
    }
}

bool isCFG() {
    for (const auto &prodSet: productions) {
        for (const auto &prod: prodSet.second) {
            if (prod.symbols.size() == 0) {
                return false;
            }
        }
    }
    return true;
}

unordered_set<string> computeFirst(const string& nonterminal) {
    if (firstSets.find(nonterminal) != firstSets.end()) {
        return firstSets[nonterminal];
    }

    unordered_set<string> result;
    for (const auto& prod : productions[nonterminal]) {
        size_t i = 0;
        while (i < prod.symbols.size() && prod.symbols[i] == "ε") {
            result.insert("ε");
            i++;
        }

        if (i < prod.symbols.size()) {
            if (terminals.find(prod.symbols[i]) != terminals.end()) {
                result.insert(prod.symbols[i]);
            } else {
                unordered_set<string> firstSet = computeFirst(prod.symbols[i]);
                result.insert(firstSet.begin(), firstSet.end());
            }
        }
    }

    firstSets[nonterminal] = result;
    return result;
}

unordered_set<string> computeFollow(const string& nonterminal) {
    if (followSets.find(nonterminal) != followSets.end()) {
        return followSets[nonterminal];
    }

    unordered_set<string> result;

    if (nonterminal == "E") {
        result.insert("$");
    }

    for (const auto& prodSet : productions) {
        for (const auto& prod : prodSet.second) {
            for (size_t i = 0; i < prod.symbols.size(); ++i) {
                if (prod.symbols[i] == nonterminal) {
                    if (i == prod.symbols.size() - 1) {
                        if (prod.nonterminal != nonterminal) {
                            unordered_set<string> followSet = computeFollow(prod.nonterminal);
                            result.insert(followSet.begin(), followSet.end());
                        }
                    } else {
                        size_t j = i + 1;
                        while (j < prod.symbols.size() && prod.symbols[j] == "ε") {
                            j++;
                        }

                        if (j < prod.symbols.size()) {
                            if (terminals.find(prod.symbols[j]) != terminals.end()) {
                                result.insert(prod.symbols[j]);
                            } else {
                                unordered_set<string> firstSet = computeFirst(prod.symbols[j]);
                                result.insert(firstSet.begin(), firstSet.end());
                            }
                        } else {
                            unordered_set<string> followSet = computeFollow(prod.nonterminal);
                            result.insert(followSet.begin(), followSet.end());
                        }
                    }
                }
            }
        }
    }

    followSets[nonterminal] = result;
    return result;
}

int main() {
    readGrammar("g1.txt");

    printNonTerminals();
    printTerminals();

    for (const auto& nonterm : nonterminals) {
        printProductions(nonterm);

        // Test FIRST set for each nonterminal
        unordered_set<string> firstSet = computeFirst(nonterm);
        cout << "FIRST(" << nonterm << "): { ";
        for (const auto& symbol : firstSet) {
            cout << symbol << " ";
        }
        cout << "}\n";

        // Test FOLLOW set for each nonterminal
        unordered_set<string> followSet = computeFollow(nonterm);
        cout << "FOLLOW(" << nonterm << "): { ";
        for (const auto& symbol : followSet) {
            cout << symbol << " ";
        }
        cout << "}\n";
    }

    return 0;
}
