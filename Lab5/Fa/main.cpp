#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>

namespace std {
    template <>
    struct hash<std::pair<std::string, char>> {
        size_t operator()(const std::pair<std::string, char>& p) const {
            size_t hash1 = std::hash<std::string>{}(p.first);
            size_t hash2 = std::hash<char>{}(p.second);
            return hash1 ^ hash2;
        }
    };
}

class FiniteAutomaton {
public:
    std::unordered_set<std::string> states;
    std::unordered_set<char> alphabet;
    std::unordered_map<std::pair<std::string, char>, std::string> transitions;
    std::string initial_state;
    std::unordered_set<std::string> final_states;

    void readFromFile() {
        std::ifstream file("FA.in");
        std::string line;
        while (std::getline(file, line)) {
            parseLine(line);
        }
    }

    void displayElements() const {
        std::cout << "1. Set of States: ";
        displaySet(states);
        std::cout << "2. Alphabet: ";
        displaySet(alphabet);
        std::cout << "3. Transitions:\n";
        displayTransitions();
        std::cout << "4. Initial State: " << initial_state << std::endl;
        std::cout << "5. Set of Final States: ";
        displaySet(final_states);
    }

    void displayStates(){
        displaySet(states);
    }

    void displayAlphabet(){
        displaySet(alphabet);
    }

    void displayInitialState(){
        std::cout << "Initial State: " << initial_state << std::endl;
    }

    void displayFinalState(){
        std::cout << "Set of Final States: ";
        displaySet(final_states);
    }

    bool isAcceptedSequence(const std::string& sequence) const {

        for(auto i:transitions){
            std::cout << i.first.first << ' ' << i.first.second << ' ' << i.second << '\n';
        }
        std::cout << "________________________\n";

        std::string current_state = initial_state;

        std::cout << initial_state << '\n';

        for (char symbol : sequence) {
            auto transition = std::make_pair(current_state, symbol);
            auto it = transitions.find(transition);

            if (it != transitions.end()) {
                current_state = it->second;
            } else {
                std::cout << "No transition found for state " << current_state << " and symbol " << symbol << '\n';
                return false;
            }
        }

        return final_states.find(current_state) != final_states.end();
    }

    void displayTransitions() const {
        for (const auto& transition : transitions) {
            std::cout << "   " << transition.first.first << " --(" << transition.first.second << ")--> " << transition.second << std::endl;
        }
    }

private:
    void parseLine(const std::string& line) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            if (key == "States") {
                states.clear(); // Clear existing states
                std::vector<std::string> stateList = split(value, ',');
                states.insert(stateList.begin(), stateList.end());
            } else if (key == "Alphabet") {
                alphabet.clear(); // Clear existing alphabet
                std::vector<std::string> alphabetList = split(value, ' ');
                for (const std::string& symbol : alphabetList) {
                    if (symbol.size() == 1) {
                        alphabet.insert(symbol[0]);
                    } else {
                        std::cerr << "Error: Invalid alphabet symbol '" << symbol << "'. Ignoring.\n";
                    }
                }
            }
            else if (key == "Transitions") {
                transitions.clear(); // Clear existing transitions
                std::vector<std::string> transitionsList = split(value, ',');
                for (const std::string& transition : transitionsList) {
                    size_t arrowPos = transition.find('-');
                    std::string start = transition.substr(1, arrowPos-1);
                    char symbol = transition[arrowPos + 1];
                    std::string end = transition.substr(arrowPos + 4);
                    transitions[std::make_pair(start, symbol)] = end;
                }
            } else if (key == "Initial State") {
                initial_state = value;
            } else if (key == "Final States") {
                final_states.clear(); // Clear existing final states
                std::vector<std::string> finalStatesList = split(value, ',');
                final_states.insert(finalStatesList.begin(), finalStatesList.end());
            }
        }
    }

    template <typename T>
    void displaySet(const T& set) const {
        for (const auto& item : set) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    std::vector<std::string> split(const std::string& str, char delimiter) const {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

int main() {
    FiniteAutomaton fa;

    fa.readFromFile();

    while (true) {
        std::cout << "\nFinite Automaton Menu:\n";
        std::cout << "1. Display Set of States\n";
        std::cout << "2. Display Alphabet\n";
        std::cout << "3. Display Transitions\n";
        std::cout << "4. Display Initial State\n";
        std::cout << "5. Display Set of Final States\n";
        std::cout << "6. Verify Sequence (DFA)\n";
        std::cout << "7. Exit\n";

        int choice;
        std::cout << "Enter your choice (1-7): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                fa.displayStates();
            case 2:
                fa.displayAlphabet();
            case 3:
                fa.displayTransitions();
            case 4:
                fa.displayInitialState();
            case 5:
                fa.displayFinalState();
                break;
            case 6: {
                std::string sequence;
                std::cout << "Enter the sequence to verify: ";
                std::cin >> sequence;
                if (fa.isAcceptedSequence(sequence)) {
                    std::cout << "Sequence is accepted by the Finite Automaton.\n";
                } else {
                    std::cout << "Sequence is not accepted by the Finite Automaton.\n";
                }
                break;
            }
            case 7:
                return 0;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 7.\n";
        }
    }

    return 0;
}
