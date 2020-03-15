//
// Created by axelzucho on 9/04/19.
//

#ifndef NFA_TO_DFA_DFA_H
#define NFA_TO_DFA_DFA_H

#include <bitset>
#include <functional>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include "NFA.h"

using namespace std;

class DFA {
private:
    // Hash function required for unordered_map with a pair of bitset and char as key.
    struct pair_bitset_char_hash {
        size_t operator()(const pair<bitset<128>, char> &k) const {
            return hash<string>()(k.first.to_string() + k.second);
        }
    };

    // Hash function required for unordered_map with a bitset key.
    struct bitset_hash {
        bool operator()(const bitset<128> &k) const {
            return (hash<string>()(k.to_string()));
        }
    };

    // Comparator function required for unordered_map with a pair of bitset and char as key.
    struct pair_bitset_char_comparator {
        bool operator()(const pair<bitset<128>, char> &a, const pair<bitset<128>, char> &b) const {
            return a.first.to_ullong() < b.first.to_ullong();
        }
    };

    // Comparator function required for unordered_map with a bitset key.
    struct bitset_comparator {
        bool operator()(const bitset<128> &a, const bitset<128> &b) const {
            return a.to_ullong() < b.to_ullong();
        }
    };

    struct bitset_equals {
        bool operator()(const bitset<128> &a, const bitset<128> &b) const {
            return a.to_ullong() == b.to_ullong();
        }
    };

    // Where the original NFA will be stored.
    NFA *nfa;

    // Amount of states fo the DFA.
    unsigned long long state_amount;

    // Alphabet for the DFA.
    set<char> alphabet;

    // A set of sets, in which each set will be one final state.
    set<bitset<128>, bitset_comparator> final_states;

    // A set that indicates the initial state for the DFA.
    bitset<128> initial_state;

    // A dictionary that maps a pair of a state and a symbol to another state.
    unordered_map<pair<bitset<128>, char>, bitset<128>, pair_bitset_char_hash, pair_bitset_char_comparator> transitions;

    // A dictionary that maps the translations between a state, represented by a set, to an int.
    unordered_map<bitset<128>, unsigned long long, bitset_hash, bitset_equals> translations;

    // A set of sets that show all the states already processed or that were already found to be processed.
    set<bitset<128>, bitset_comparator> queued_states;

    // A queue of states which are yet to process.
    queue<bitset<128>> states_to_search;

    // Function that adds the epsilon paths from a given states to the given set of states.
    void set_epsilon_path(bitset<128> &states, unsigned long long state_to_add);

    // Function that adds all epsilon paths all the states in the given set.
    void set_epsilon_path(bitset<128> &states_no_epsilon);

    // Finds all the transitions from the given nfa.
    void find_transitions();

    // Finds all final states from the given nfa.
    void find_final_states();

    // Updates the translations map value of the object.
    void update_translations();

public:
    explicit DFA(NFA &nfa);

    // Writes the DFA to the given filepath.
    bool write_to_file(string filepath);
};

#endif //NFA_TO_DFA_DFA_H
