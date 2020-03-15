//
// Created by axelzucho on 9/04/19.
//

#include <fstream>
#include <iostream>
#include "NFA.h"

using namespace std;

NFA::NFA(const string &filepath)
{
    fstream file;
    file.open(filepath);
    // Check if we could open the provided filepath.
    if (!file.is_open())
    {
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

    unsigned long long final_state_amount;

    // Gets the amount of final states.
    file >> final_state_amount;
    // Iterates for each of the final states and adds them to the vector.
    for (unsigned long long i = 0; i < final_state_amount; ++i)
    {
        int state;
        file >> state;
        // TODO (for 1 << 128 which bitset currently supports we need boost/multiprecision)
        // https://stackoverflow.com/questions/35506314/c-shift-left-with-big-value
        this->final_states |= (1 << state);
    }

    unsigned long long transition_amount;

    file >> transition_amount;
    // Iterates for each transition in the NFA.
    for (unsigned long long i = 0; i < transition_amount; ++i)
    {
        int initial_node, final_node;
        char symbol;
        // Gets the transition data from the file.
        file >> initial_node >> final_node >> symbol;
        // Create a pair that will function as the key in our map.
        add_edge(initial_node, final_node, symbol);
    }

    file.close();
}

void NFA::add_edge(int initial_node, int final_node, char symbol)
{
    pair<int, char> key = make_pair(initial_node, symbol);
    // Add the new destination to the bitset.
    this->transitions[key] |= (1 << final_node);
}

bool NFA::found_file()
{
    return opened_file;
}
