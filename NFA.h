//
// Created by axelzucho on 9/04/19.
//

#ifndef NFA_TO_DFA_NFA_H
#define NFA_TO_DFA_NFA_H

#include <vector>
#include <map>

using namespace std;

class NFA {
private:
    int state_amount;
    int alphabet_size;
    int initial_node;
    vector<int> final_states;
    map<pair<int,char>, vector<int>> transitions;

    friend class DFA;
public:
    explicit NFA(string filepath);

};


#endif //NFA_TO_DFA_NFA_H
