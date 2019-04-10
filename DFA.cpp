//
// Created by axelzucho on 9/04/19.
//

#include "DFA.h"

DFA::DFA(NFA& nfa) {
    this->nfa = &nfa;
    this->alphabet_size = nfa.alphabet_size;

    this->set_epsilon_path(this->initial_states, nfa.initial_node);
    this->states_to_search.push(initial_states);
    this->queued_states.insert(initial_states);
    this->find_transitions();
}

void DFA::set_epsilon_path(set<int> &states, int state_to_add) {
    states.insert(state_to_add);

    for(auto it:this->nfa->transitions[make_pair(state_to_add, 'f')]){
        if(states.find(it) != states.end()){
            continue;
        } else {
            set_epsilon_path(states, it);
        }
    }
}

void DFA::set_epsilon_path(set<int> &states_no_epsilon) {
    for(auto it:states_no_epsilon){
        set_epsilon_path(states_no_epsilon, it);
    }
}

void DFA::find_transitions() {
    while(!this->states_to_search.empty()){
        set<int> processing_states = this->states_to_search.front();
        this->states_to_search.pop();

        this->set_epsilon_path(processing_states);

        for(int i = 0; i < this->alphabet_size; i++){
            char symbol = 'a' + i;
            set<int> new_states;
            for(auto it:processing_states){
                if(nfa->transitions.find(make_pair(it, symbol)) == nfa->transitions.end()) {
                    continue;
                } else{
                    for(auto state:nfa->transitions[make_pair(it, symbol)]) {
                        new_states.insert(state);
                    }
                }
            }
            if(new_states.empty()) continue;

            this->set_epsilon_path(new_states);

            this->transitions[make_pair(processing_states, symbol)] = new_states;
            if(this->queued_states.find(new_states) == this->queued_states.end()){
                this->queued_states.insert(new_states);
                this->states_to_search.push(new_states);
            }
        }
    }
}
