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
    NFA* nfa;
    int state_amount;
    int alphabet_size;
    set<int> final_states;
    set<int> initial_states;
    map<pair<set<int>, char>, set<int>> transitions;
    set<set<int>> queued_states;
    queue<set<int>> states_to_search;
public:
    explicit DFA(NFA& nfa);
    void set_epsilon_path(set<int>& states, int state_to_add);
    void set_epsilon_path(set<int>& states_no_epsilon);
    void find_transitions();
};


#endif //NFA_TO_DFA_DFA_H