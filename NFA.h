//
// Created by axelzucho on 9/04/19.
//

#ifndef NFA_TO_DFA_NFA_H
#define NFA_TO_DFA_NFA_H

#include <bitset>
#include <functional>
#include <set>
#include <string>
#include <unordered_map>

using namespace std;

class NFA
{
private:
    // Hash function required for unordered_map with a pair of unsigned long long and char.
    struct pair_llu_char_hash
    {
        size_t operator()(const pair<unsigned long long, char> &k) const
        {
            return (hash<string>()(to_string(k.first) + k.second));
        }
    };

    // Shows if the input file could be opened.
    bool opened_file;

    // The amount of states in the NFA
    unsigned long long state_amount;

    // The alphabet for the NFA.
    set<char> alphabet;

    // The initial node (by definition always 0).
    unsigned long long initial_node;

    // Contains all final states in the DFA.
    bitset<128> final_states;

    // Dictionary that maps a state and a symbol to a set of states.
    unordered_map<pair<unsigned long long, char>, bitset<128>, pair_llu_char_hash> transitions;

    void add_edge(int initial_node, int final_node, char symbol);

    // So that DFA can access private values.
    friend class DFA;
    friend class Regex;

public:
    explicit NFA(const string &filepath);
    // Returns if the file could be found.
    explicit NFA() = default;
    bool found_file();
};

#endif //NFA_TO_DFA_NFA_H
