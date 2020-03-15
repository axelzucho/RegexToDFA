//
// Created by axelzucho on 9/04/19.
//

#include "DFA.h"

#include <fstream>
#include <iostream>

DFA::DFA(NFA &nfa)
{
    this->nfa = &nfa;
    this->alphabet_size = nfa.alphabet_size - 1;

    // Obtains the epsilon path from the initial node and saves the group of states to the initial_state class attribute.
    this->set_epsilon_path(this->initial_state, nfa.initial_node);

    // Adds the initial state to the state queue to find transitions.
    this->states_to_search.push(bitset<128>(initial_state));

    // Inserts the initial state in the set to signal that it was already queued.
    this->queued_states.insert(bitset<128>(initial_state));

    // Finds all the DFA's transitions.
    this->find_transitions();

    // Gets the final state amount.
    this->state_amount = (unsigned long long)this->queued_states.size();

    // Finds all the final states.
    this->find_final_states();
}

void DFA::set_epsilon_path(bitset<128> &states, unsigned long long state_to_add)
{
    // Inserts the state to add into the bitset.
    states |= (1 << state_to_add);

    // Iterates all the states that can be reached with an epsilon movement.
    bitset<128> reachable_states = this->nfa->transitions[make_pair(state_to_add, 'f')];
    for (unsigned long long i = 0; i < reachable_states.size(); ++i)
    {
        // Call the same function recursively to add the new state and all its epsilon movements.
        if (reachable_states[i] && !states[i])
        {
            set_epsilon_path(states, i);
        }
    }
}

void DFA::set_epsilon_path(bitset<128> &states_no_epsilon)
{
    // Iterates all the states in the bitset.
    for (unsigned long long i = 0; i < states_no_epsilon.size(); ++i)
    {
        // Calls the function to add the epsilon path for the given state into the bitset.
        if (states_no_epsilon[i])
            set_epsilon_path(states_no_epsilon, i);
    }
}

void DFA::find_transitions()
{
    // While there are still elements in the queue.
    while (!this->states_to_search.empty())
    {
        bitset<128> processing_states = this->states_to_search.front();
        this->states_to_search.pop();

        // Gets the epsilon path for the analyzing state.
        this->set_epsilon_path(processing_states);

        // Iterates through all symbols in the alphabet.
        for (unsigned long long char_offset = 0; char_offset < this->alphabet_size; ++char_offset)
        {
            // By definition, this is always the alphabet.
            char symbol = (char)('a' + char_offset);
            // Bitset that will contain the new states accessible from the current state and the iterating symbol.
            bitset<128> new_states;

            // For each state in the current set of states.
            for (unsigned long long i = 0; i < processing_states.size(); ++i)
            {
                // Inserts all the states accessible with the given symbol.
                if (processing_states[i] && nfa->transitions.find(make_pair(i, symbol)) != nfa->transitions.end())
                {
                    bitset<128> accessible_states = nfa->transitions[make_pair(i, symbol)];
                    for (unsigned long long j = 0; j < accessible_states.size(); ++j)
                    {
                        if (accessible_states[j])
                            new_states |= (1 << j);
                    }
                }
            }

            // If there are no states accessible from the current state with the given symbol, just continue.
            if (new_states.none())
                continue;

            // We set the epsilon path of this new states.
            this->set_epsilon_path(new_states);

            // We add this transition.
            this->transitions[make_pair(processing_states, symbol)] = new_states;
            // If this is the first time we see this set of states, we need to add it to the queue and the set.
            if (this->queued_states.find(new_states) == this->queued_states.end())
            {
                this->queued_states.insert(new_states);
                this->states_to_search.push(new_states);
            }
        }
    }
}

void DFA::find_final_states()
{
    // Iterates all states in the DFA.
    for (auto queued_state : this->queued_states)
    {
        for (int i = 0; i < queued_state.size(); ++i)
        {
            if (queued_state[i] && this->nfa->final_states[i])
            {
                this->final_states.insert(queued_state);
                break;
            }
        }
    }
}

void DFA::update_translations()
{
    this->translations.clear();

    // By definition, 0 is always the initial state.
    this->translations[this->initial_state] = 0;
    // We start the translations in 1.
    unsigned long long current_index = 1;
    // Iterates all states in the DFA.
    for (const auto &states : this->queued_states)
    {
        // If the given state hasn't been translated, add an integer translation.
        if (translations.find(states) == translations.end())
        {
            translations[states] = current_index++;
        }
    }
}

bool DFA::write_to_file(string filepath)
{
    // We need to update the translations from set of states to an int.
    this->update_translations();

    ofstream file;
    file.open(filepath);
    // Check if we could open the provided filepath, if not, return false.
    if (!file.is_open())
    {
        cout << "File to write results not opened correctly" << endl;
        return false;
    }

    // Add the first three lines to the file.
    file << this->state_amount << "\n";
    file << this->alphabet_size << "\n";
    file << this->final_states.size() << "\n";

    // Iterates through all the final states and adds them to the file.
    for (const auto &final_state : final_states)
    {
        file << this->translations[final_state] << "\n";
    }

    // The amount of transitions.
    file << transitions.size() << "\n";

    // Iterates through all transitions and adds each of them to the file.
    for (const auto &transition : transitions)
    {
        file << translations[transition.first.first] << " " << translations[transition.second] << " " << transition.first.second << "\n";
    }

    file.close();

    // Return that the file was written successfully.
    return true;
}