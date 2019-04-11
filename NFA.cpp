//
// Created by axelzucho on 9/04/19.
//

#include <fstream>
#include <iostream>
#include "NFA.h"

using namespace std;

NFA::NFA(string filepath) {
    fstream file;
    file.open(filepath);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        opened_file = false;
        return;
    }
    opened_file = true;

    this->initial_node = 0;

    file >> this->state_amount;
    file >> this->alphabet_size;

    unsigned long final_state_amount;

    file >> final_state_amount;
    this->final_states.reserve(final_state_amount);
    for(int i = 0; i < final_state_amount; ++i){
        int state;
        file >> state;
        this->final_states.push_back(state);
    }

    unsigned long transition_amount;

    file >> transition_amount;
    for(int i = 0; i < transition_amount; ++i){
        int initial_node, final_node;
        char symbol;
        file >> initial_node >> final_node >> symbol;
        pair<int, char> key = make_pair(initial_node, symbol);
        if (this->transitions.find(key) != transitions.end()){
            transitions[key].push_back(final_node);
        } else {
            this->transitions[key] = {final_node};
        }
    }

    file.close();
}

bool NFA::found_file() {
    return opened_file;
}