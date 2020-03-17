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

const vector<int> NFA::get_nodes_vector() {
  vector<int> v(state_amount);
  iota(begin(v), end(v), 0);
  return v;
}

const vector<pair<int, int>> NFA::get_edges_vector() {
  vector<pair<int, int>> v;
  //for (auto& it: transitions) {
    //cout << it.first.first << " " << it.first.second;
    //cout << it.first.second.to_string();
  //}
  return v;
}

vector<char> NFA::get_transitions() {
  vector<char> v;

  return v;
}

unordered_set<int> NFA::get_final_states() {
  unordered_set<int> final_set;

  return final_set;
}

void NFA::graph(string output_file)
{
    //Set up the nodes, edges, and transition symbols
    const vector<int> nodes = get_nodes_vector();
    const vector<pair<int, int>> edges_no_trans = get_edges_vector();   //The start and end states of each edge
    /*vector<char> transitions = get_transitions();                       //The transition symbol for ^
    const int n_edges = edges_no_trans.size();
    unordered_set<int> final_states_set = get_final_states();           //For determining whether a state should be displayed with a double circle

    struct Vertex
    {
        int id;
        string shape;
    };
    struct Edge
    {
        char weight;
    };

    //Contains all the info about the graph
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, Vertex, Edge> DirectedGraph;

    DirectedGraph g;

    //For determining which shape a state should assume (circle/double circle)
    string vertex1_circle_shape;
    string vertex2_circle_shape;

    //Populate the graph with the edges
    for (int i = 0; i < n_edges; i++)
    {
        //Determine the shape of the vertices (depending on whether they're final or not)
        if(final_states_set.find(edges_no_trans[i].first) == final_states_set.end()) {
          vertex1_circle_shape = "circle";        //If the state/vertex isn't found in the final state set
        } else {
          vertex1_circle_shape = "doublecircle";
        }

        if(final_states_set.find(edges_no_trans[i].second) == final_states_set.end()) {
          vertex_circle_shape = "circle";
        } else {
          vertex_circle_shape = "doublecircle";
        }
        add_edge(add_vertex(Vertex{edges_no_trans[i].first, edge1_circle_shape}, g), add_vertex(Vertex{edges_no_trans[i].second, edge2_circle_shape}, g), Edge{transitions[i]}, g);
    }

    //Write graph to file
    {
        ofstream f(output_file + ".dot");
        boost::dynamic_properties dp;
        dp.property("node_id", get(&Vertex::id, g));
        dp.property("shape", get(&Vertex::shape, g));
        dp.property("label", get(&Edge::weight, g));
        write_graphviz_dp(f, g, dp);
        f.close();
    }

    //Convert the .dot to .png
    string dot_to_png = "dot -Tpng " + output_file + ".dot > " + output_file + ".png";
    std::system(dot_to_png.c_str());
    cout << "Graph exported as " << output_file << ".dot and " << output_file << ".png" << endl;*/
}
