
#include "Graph.h"

#include <iostream>

Graph::Graph(DFA &dfa) {
  this->nfa = NULL;
  this->dfa = &dfa;
  state_amount = this->dfa->get_state_amount();
  transition_amount = this->dfa->get_transition_amount();
  transitions = this->dfa->get_transitions();
}

Graph::Graph(NFA &nfa) {
  this->dfa = NULL;
  this->nfa = &nfa;
  state_amount = this->nfa->get_state_amount();
}

void Graph::graph_DFA() {
  //Check if there's a DFA to graph
  if(this->dfa == NULL) {
    cout << "No DFA available" << endl;
    return;
  }


}

void Graph::graph_NFA() {
  if(this->nfa == NULL) {
    cout << "No NFA available" << endl;
    return;
  }

  Edge edge_array[state_amount];
}
