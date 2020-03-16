//
// Created by axelzucho on 9/04/19.
//

#include <fstream>
#include "NFA.h"


using namespace std;

NFA::NFA(const string &filepath) {
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

    unsigned long long final_state_amount;

    // Gets the amount of final states.
    file >> final_state_amount;
    // Iterates for each of the final states and adds them to the vector.
    for (unsigned long long i = 0; i < final_state_amount; ++i) {
        int state;
        file >> state;
        // TODO (for 1 << 128 which bitset currently supports we need boost/multiprecision)
        // https://stackoverflow.com/questions/35506314/c-shift-left-with-big-value
        this->final_states |= (1 << state);
    }

    unsigned long long transition_amount;

    file >> transition_amount;
    // Iterates for each transition in the NFA.
    for (unsigned long long i = 0; i < transition_amount; ++i) {
        int initial_node, final_node;
        char symbol;
        // Gets the transition data from the file.
        file >> initial_node >> final_node >> symbol;
        // Create a pair that will function as the key in our map.
        add_edge(initial_node, final_node, symbol);
        alphabet.insert(symbol);
    }

    file.close();
}

void NFA::add_edge(int initial_node, int final_node, char symbol) {
    pair<int, char> key = make_pair(initial_node, symbol);
    // Add the new destination to the bitset.
    this->transitions[key] |= (1 << final_node);
}

bool NFA::found_file() {
    return opened_file;
}
/*
unsigned long long NFA::get_state_amount() {
  return state_amount;
}

//Returns a vector containing each node
const vector<int> NFA::get_nodes_vector() {
  vector<int> v(state_amount) ;
  iota (begin(v), end(v), 0);
  return v;
}

//Returns the start and end node of every edge as a vector of pairs
const vector<pair<int,int>> NFA::get_edges_vector() {
  vector<pair<int,int>> v;
  for (const auto &transition : transitions) {
      v.push_back(make_pair(translations[transition.first.first], translations[transition.second]));
  }
  return v;
}

//This returns a vector of transitions, each transition symbol refers to an edge in the edge vector
vector<char> NFA::get_transitions() {
  vector<char> v;
  for (const auto &transition : transitions) {
      v.push_back(transition.first.second);
  }
  return v;
}

void NFA::graph(string output_file) {
  //Set up the nodes, edges, and transition symbols
  const vector<int> nodes = get_nodes_vector();
  const vector<pair<int,int>> edges_no_trans = get_edges_vector();
  vector<char> transitions = get_transitions();
  const int n_edges = edges_no_trans.size();

  typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, EdgeWeightProperty> DirectedGraph;
  DirectedGraph g;

  //Populate the graph with the edges
  for(int i = 0; i < n_edges; i++) {
    add_edge(edges_no_trans[i].first, edges_no_trans[i].second, transitions[i], g);
  }

  //Write graph to file
  {
    ofstream f(output_file + ".dot");
    write_graphviz(f, g
    );
    f.close();
  }

  string dot_to_png = "dot -Tpng " + output_file + ".dot > " + output_file + ".png";
  std::system(dot_to_png.c_str());
  cout << "Graph exported as " << output_file << ".dot and " << output_file << ".png" << endl;
}*/
