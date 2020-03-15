//
// Created by axelzucho on 9/04/19.
//

#include "DFA.h"

#include "SpecialCharacters.h"

#include <fstream>
#include <iostream>

DFA::DFA(NFA& nfa) {
    this->nfa = &nfa;
    this->alphabet_size = nfa.alphabet_size - 1;

    // Obtains the epsilon path from the initial node and saves the group of states to the initial_state class attribute.
    this->set_epsilon_path(this->initial_state, nfa.initial_node);

    // Adds the initial state to the state queue to find transitions.
    this->states_to_search.push(initial_state);

    // Inserts the initial state in the set to signal that it was already queued.
    this->queued_states.insert(initial_state);

    // Finds all the DFA's transitions.
    this->find_transitions();

    // Gets the final state amount.
    this->state_amount = (int)this->queued_states.size();

    // Finds all the final states.
    this->find_final_states();
}

void DFA::set_epsilon_path(set<int> &states, int state_to_add) {
    // Inserts the state to add into the set.
    states.insert(state_to_add);

    // Iterates all the states that can be reached with an epsilon movement.
    for(auto it:this->nfa->transitions[make_pair(state_to_add, EPSILON)]){
        // If the state was already in the set, just continue.k
        if(states.find(it) != states.end()){
            continue;
        } else {
            // Call the same function recursively to add the new state and all its epsilon movements.
            set_epsilon_path(states, it);
        }
    }
}

void DFA::set_epsilon_path(set<int> &states_no_epsilon) {
    // Iterates all the states in the set.
    for(auto it:states_no_epsilon){
        // Calls the function to add the epsilon path for the given state into the set.
        set_epsilon_path(states_no_epsilon, it);
    }
}

void DFA::find_transitions() {
    // While there are still elements in the queue.
    while(!this->states_to_search.empty()){
        set<int> processing_states = this->states_to_search.front();
        this->states_to_search.pop();

        // Gets the epsilon path for the analyzing state.
        this->set_epsilon_path(processing_states);

        // Iterates through all symbols in the alphabet.
        for(int i = 0; i < this->alphabet_size; i++){
            // By definition, this is always the alphabet.
            char symbol = (char)('a' + i);
            // Set that will contain the new states accessible from the current state and the iterating symbol.
            set<int> new_states;
            // For each state in the current set of states.
            for(auto it:processing_states){
                // If we don't have any path with the given symbol, just continue.
                if(nfa->transitions.find(make_pair(it, symbol)) == nfa->transitions.end()) {
                    continue;
                } else{
                    // Inserts all the states accessible with the given symbol.
                    for(auto state:nfa->transitions[make_pair(it, symbol)]) {
                        new_states.insert(state);
                    }
                }
            }
            // If there are no states accessible from the current state with the given symbol, just continue.
            if(new_states.empty()) continue;

            // We set the epsilon path of this new states.
            this->set_epsilon_path(new_states);

            // We add this transition.
            this->transitions[make_pair(processing_states, symbol)] = new_states;
            // If this is the first time we see this set of states, we need to add it to the queue and the set.
            if(this->queued_states.find(new_states) == this->queued_states.end()){
                this->queued_states.insert(new_states);
                this->states_to_search.push(new_states);
            }
        }
    }
}

void DFA::find_final_states() {
    // Iterates all states in the DFA.
    for(auto queued_state:this->queued_states){
        // Iterates each state of the final states.
        for(const auto& final_state:this->nfa->final_states){
            // If the current state contains the final state being checked, add it to the set of final states.
            if(queued_state.find(final_state) != queued_state.end()){
                this->final_states.insert(queued_state);
                break;
            }
        }
    }
}

void DFA::update_translations() {
    this->translations.clear();

    // By definition, 0 is always the initial state.
    this->translations[this->initial_state] = 0;
    // We start the translations in 1.
    int current_index = 1;
    // Iterates all states in the DFA.
    for(const auto& states:this->queued_states){
        // If the given state hasn't been translated, add an integer translation.
        if(translations.find(states) == translations.end()){
            translations[states] = current_index++;
        }
    }
}

bool DFA::write_to_file(string filepath) {
    // We need to update the translations from set of states to an int.
    this->update_translations();

    ofstream file;
    file.open(filepath);
    // Check if we could open the provided filepath, if not, return false.
    if (!file.is_open()) {
        cout << "File to write results not opened correctly" << endl;
        return false;
    }

    // Add the first three lines to the file.
    file << this->state_amount << "\n";
    file << this->alphabet_size << "\n";
    file << this->final_states.size() << "\n";

    // Iterates through all the final states and adds them to the file.
    for(const auto& final_state:final_states){
        file << this->translations[final_state] << "\n";
    }

    // The amount of transitions.
    file << transitions.size() << "\n";

    // Iterates through all transitions and adds each of them to the file.
    for(const auto& transition:transitions){
        file << translations[transition.first.first] << " " << translations[transition.second] << " " << transition.first.second << "\n";
    }

    file.close();

    // Return that the file was written successfully.
    return true;
}