//
// Created by axelzucho on 9/04/19.
//

#ifndef NFA_TO_DFA_NFA_H
#define NFA_TO_DFA_NFA_H

#include <set>
#include <map>

using namespace std;

class NFA {
private:
    // Shows if the input file could be opened.
    bool opened_file;

    // The amount of states in the NFA
    int state_amount;

    // The alphabet size for the NFA.
    int alphabet_size;

    // The initial node (by definition always 0).
    int initial_node;

    // Contains all final states in the DFA.
    set<int> final_states;

    // Dictionary that maps a state and a symbol to a set of states.
    map<pair<int,char>, set<int>> transitions;

    // So that DFA can access private values.
    friend class DFA;
public:
    explicit NFA(string filepath);
    // Returns if the file could be found.
    bool found_file();

};


#endif //NFA_TO_DFA_NFA_H
