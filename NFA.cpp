//
// Created by axelzucho on 9/04/19.
//

#include <fstream>
#include <iostream>
#include "NFA.h"
#include "SpecialCharacters.h"


using namespace std;

NFA::NFA(const string& filepath) {
    fstream file;
    file.open(filepath);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        opened_file = false;
        return;
    }
    // We could open the file.
    opened_file = true;

    // The initial node by definition is always 0.
    this->initial_node = 0;

    // Get the amount of states.
    file >> this->state_amount;
    // Get the alphabet size.
    file >> this->alphabet_size;

    int final_state_amount;

    // Gets the amount of final states.
    file >> final_state_amount;
    // Iterates for each of the final states and adds them to the vector.
    for(int i = 0; i < final_state_amount; ++i){
        int state;
        file >> state;
        this->final_states.insert(state);
    }

    int transition_amount;

    file >> transition_amount;
    // Iterates for each transition in the NFA.
    for(int i = 0; i < transition_amount; ++i){
        int initial_node, final_node;
        char symbol;
        // Gets the transition data from the file.
        file >> initial_node >> final_node >> symbol;
        // Create a pair that will function as the key in our map.
        add_edge(initial_node, final_node, symbol);
    }

    file.close();
}

void NFA::add_edge(int initial_node, int final_node, char symbol){
    pair<int, char> key = make_pair(initial_node, symbol);
    // If there are already existing transitions for the given key, just add the new destination to the set.
    if (this->transitions.find(key) != transitions.end()){
        transitions[key].insert(final_node);
    } else {
        // If not, add a new set containing the destination state.
        this->transitions[key] = {final_node};
    }
}

bool NFA::found_file() {
    return opened_file;
}

