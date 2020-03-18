//
// Created by axelzucho on 9/04/19.
//

#include <fstream>
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

  unsigned long long final_state_amount;

  // Gets the amount of final states.
  file >> final_state_amount;
  // Iterates for each of the final states and adds them to the vector.
  for (unsigned long long i = 0; i < final_state_amount; ++i)
  {
    unsigned long long state;
    file >> state;
    this->final_states[state] = 1;
  }

  unsigned long long transition_amount;

  file >> transition_amount;
  // Iterates for each transition in the NFA.
  for (unsigned long long i = 0; i < transition_amount; ++i)
  {
    unsigned long long initial_node, final_node;
    char symbol;
    // Gets the transition data from the file.
    file >> initial_node >> final_node >> symbol;
    // Create a pair that will function as the key in our map.
    add_edge(initial_node, final_node, symbol);
    alphabet.insert(symbol);
  }

  file.close();
}

void NFA::add_edge(unsigned long long initial_node, unsigned long long final_node, char symbol)
{
  pair<unsigned long long, char> key = make_pair(initial_node, symbol);
  // Add the new destination to the bitset.
  this->transitions[key][final_node] = 1;
}

bool NFA::found_file()
{
  return opened_file;
}

//Get a vector of nodes
const vector<unsigned long long> NFA::get_nodes_vector()
{
  vector<unsigned long long> v(state_amount);
  iota(begin(v), end(v), 0); //Generate a vector of consecutive integers
  return v;
}

//Get a vector of edges with transitions
edge_transitions NFA::get_edges_vector()
{
  edge_transitions e_t;
  //For each element in the transitions map
  for (const auto &elem : transitions)
  {
    unsigned long long currentNode = elem.first.first;
    char transition = elem.first.second;
    //A 1 in the bitset indicates a transition to that node
    for (unsigned long long i = 0; i < state_amount; i++)
    {
      if (elem.second[i])
      {
        e_t.edges_vector.push_back(make_pair(currentNode, i));
        e_t.transitions_vector.push_back(transition);
      }
    }
  }
  return e_t;
}

//Returns a vector of final states
unordered_set<unsigned long long> NFA::get_final_states()
{
  unordered_set<unsigned long long> final_set;
  for (unsigned long long i = 0; i < state_amount; i++)
  {
    //A 1 in the bitset means the node is final
    if (final_states[i])
    {
      final_set.insert(i);
    }
  }
  return final_set;
}

void NFA::graph(string output_file)
{
  //Set up the nodes, edges, and transition symbols
  const vector<unsigned long long> nodes = get_nodes_vector();
  edge_transitions edges_trans = get_edges_vector();                       //The start and end states of each edge along with its transition
  const unsigned long long n_edges = edges_trans.edges_vector.size();      //Number of edges
  unordered_set<unsigned long long> final_states_set = get_final_states(); //For determining whether a state should be displayed with a double circle

  /*
    for(unsigned long long i = 0; i < n_edges; i++) {
      cout << edges_trans.edges_vector[i].first << " " << edges_trans.edges_vector[i].second << " " << edges_trans.transitions_vector[i] << endl;
    }*/

  struct Vertex
  {
    unsigned long long id;
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
  for (unsigned long long i = 0; i < n_edges; i++)
  {
    //Determine the shape of the vertices (depending on whether they're final or not)
    if (final_states_set.find(edges_trans.edges_vector[i].first) == final_states_set.end())
    {
      vertex1_circle_shape = "circle"; //If the state/vertex isn't found in the final state set
    }
    else
    {
      vertex1_circle_shape = "doublecircle";
    }

    if (final_states_set.find(edges_trans.edges_vector[i].second) == final_states_set.end())
    {
      vertex2_circle_shape = "circle";
    }
    else
    {
      vertex2_circle_shape = "doublecircle";
    }
    boost::add_edge(add_vertex(Vertex{edges_trans.edges_vector[i].first, vertex1_circle_shape}, g), add_vertex(Vertex{edges_trans.edges_vector[i].second, vertex2_circle_shape}, g), Edge{edges_trans.transitions_vector[i]}, g);
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
  string remove_dot = output_file + ".dot";
  remove(remove_dot.c_str());
  cout << "Graph exported as " << output_file << ".png" << endl;
}
