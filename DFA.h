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
#include <vector>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

//Boost for graphing
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

#include "NFA.h"

using namespace std;

class DFA
{
private:
    static bool is_a_bigger_than_b(const bitset<12345> &a, const bitset<12345> &b)
    {
        for (long long i = b.size() - 1; i >= 0; i--)
        {
            if (a[i] ^ b[i]) {
                return a[i];
            }
        }
        return false;
    }

    static bool is_a_equal_to_b(const bitset<12345> &a, const bitset<12345> &b)
    {
        return a == b;
    }

    // Hash function required for unordered_map with a pair of bitset and char as key.
    struct pair_bitset_char_hash
    {
        size_t operator()(const pair<bitset<12345>, char> &k) const
        {
            size_t h1 = hash<bitset<12345>>()(k.first);
            size_t h2 = k.second;

            return h1 ^ (h2 << 1);
        }
    };

    // Hash function required for unordered_map with a bitset key.
    struct bitset_hash
    {
        bool operator()(const bitset<12345> &k) const
        {
            return hash<bitset<12345>>()(k);
        }
    };

    // Comparator function required for unordered_map with a pair of bitset and char as key.
    struct pair_bitset_char_comparator
    {
        bool operator()(const pair<bitset<12345>, char> &a, const pair<bitset<12345>, char> &b) const
        {
            return is_a_equal_to_b(a.first, b.first) && a.second == b.second;
        }
    };

    // Comparator function required for unordered_map with a bitset key.
    struct bitset_comparator
    {
        bool operator()(const bitset<12345> &a, const bitset<12345> &b) const
        {
            return is_a_bigger_than_b(b, a);
        }
    };

    struct bitset_equals
    {
        bool operator()(const bitset<12345> &a, const bitset<12345> &b) const
        {
            return a == b;
        }
    };

    // Where the original NFA will be stored.
    NFA *nfa;

    // Amount of states for the DFA.
    unsigned long long state_amount;

    // Alphabet for the DFA.
    set<char> alphabet;

    // A set of sets, in which each set will be one final state.
    set<bitset<12345>, bitset_comparator> final_states;

    // A set that indicates the initial state for the DFA.
    bitset<12345> initial_state;

    // A dictionary that maps a pair of a state and a symbol to another state.
    unordered_map<pair<bitset<12345>, char>, bitset<12345>, pair_bitset_char_hash, pair_bitset_char_comparator> transitions;

    // A dictionary that maps the translations between a state, represented by a set, to an int.
    unordered_map<bitset<12345>, unsigned long long, bitset_hash, bitset_equals> translations;

    // A set of sets that show all the states already processed or that were already found to be processed.
    set<bitset<12345>, bitset_comparator> queued_states;

    // A queue of states which are yet to process.
    queue<bitset<12345>> states_to_search;

    // Function that adds the epsilon paths from a given states to the given set of states.
    void set_epsilon_path(bitset<12345> &states, unsigned long long state_to_add);

    // Function that adds all epsilon paths all the states in the given set.
    void set_epsilon_path(bitset<12345> &states_no_epsilon);

    // Finds all the transitions from the given nfa.
    void find_transitions();

    // Finds all final states from the given nfa.
    void find_final_states();

    // Updates the translations map value of the object.
    void update_translations();

public:
    explicit DFA(NFA &nfa);

    //Graph the DFA
    void graph(string output_file);

    //Returns a vector of nodes in the DFA
    const vector<unsigned long long> get_nodes_vector();

    //Returns a vector of start and end node pairs
    const vector<pair<unsigned long long, unsigned long long>> get_edges_vector();

    //Returns a vector of transition symbols
    vector<char> get_transitions();

    //Returns the nodes that are final states
    unordered_set<unsigned long long> get_final_states();

    // Writes the DFA to the given filepath.
    bool write_to_file(string filepath);

    // Checks if a given chain is valid for the DFA.
    bool checkIfValid(const string &chain);
};

#endif //NFA_TO_DFA_DFA_H
