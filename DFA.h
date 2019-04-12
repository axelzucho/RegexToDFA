//
// Created by axelzucho on 9/04/19.
//

#ifndef NFA_TO_DFA_DFA_H
#define NFA_TO_DFA_DFA_H

#include <vector>
#include <map>
#include <set>
#include <queue>
#include "NFA.h"

using namespace std;

class DFA {
private:
    // Where the original NFA will be stored.
    NFA* nfa;

    // Amount of states fo the DFA.
    int state_amount;

    // Alphabet size of the DFA.
    int alphabet_size;

    // A set of sets, in which each set will be one final state.
    set<set<int>> final_states;

    // A set that indicates the initial state for the DFA.
    set<int> initial_state;

    // A dictionary that maps a pair of a state and a symbol to another state.
    map<pair<set<int>, char>, set<int>> transitions;

    // A dictionary that maps the translations between a state, represented by a set, to an int.
    map<set<int>, int> translations;

    // A set of sets that show all the states already processed or that were already found to be processed.
    set<set<int>> queued_states;

    // A queue of states which are yet to process.
    queue<set<int>> states_to_search;

    // Function that adds the epsilon paths from a given states to the given set of states.
    void set_epsilon_path(set<int>& states, int state_to_add);

    // Function that adds all epsilon paths all the states in the given set.
    void set_epsilon_path(set<int>& states_no_epsilon);

    // Finds all the transitions from the given nfa.
    void find_transitions();

    // Finds all final states from the given nfa.
    void find_final_states();

    // Updates the translations map value of the object.
    void update_translations();
public:
    explicit DFA(NFA& nfa);

    // Writes the DFA to the given filepath.
    bool write_to_file(string filepath);
};


#endif //NFA_TO_DFA_DFA_H
