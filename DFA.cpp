//
// Created by axelzucho on 9/04/19.
//

#include "DFA.h"

#include <fstream>
#include <iostream>

DFA::DFA(NFA &nfa)
{
    this->nfa = &nfa;

    for (const auto &c : nfa.alphabet)
    {
        // If the character isn't the EPSILON character, add it to our alphabet.
        if (c != '~')
        {
            alphabet.insert(c);
        }
    }

    // Obtains the epsilon path from the initial node and saves the group of states to the initial_state class attribute.
    this->set_epsilon_path(this->initial_state, nfa.initial_node);

    // Adds the initial state to the state queue to find transitions.
    this->states_to_search.push(bitset<12345>(initial_state));

    // Inserts the initial state in the set to signal that it was already queued.
    this->queued_states.insert(bitset<12345>(initial_state));

    // Finds all the DFA's transitions.
    this->find_transitions();

    // Gets the final state amount.
    this->state_amount = (unsigned long long)this->queued_states.size();

    // Finds all the final states.
    this->find_final_states();
}

void DFA::set_epsilon_path(bitset<12345> &states, unsigned long long state_to_add)
{
    // Inserts the state to add into the bitset.
    states[state_to_add] = 1;

    // Iterates all the states that can be reached with an epsilon movement.
    bitset<12345> reachable_states = this->nfa->transitions[make_pair(state_to_add, '~')];
    for (unsigned long long i = 0; i < reachable_states.size(); ++i)
    {
        // Call the same function recursively to add the new state and all its epsilon movements.
        if (reachable_states[i] && !states[i])
        {
            set_epsilon_path(states, i);
        }
    }
}

void DFA::set_epsilon_path(bitset<12345> &states_no_epsilon)
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
        bitset<12345> processing_states = this->states_to_search.front();
        this->states_to_search.pop();

        // Gets the epsilon path for the analyzing state.
        this->set_epsilon_path(processing_states);

        // Iterates through all symbols in the alphabet.
        for (const auto &symbol : alphabet)
        {
            // Bitset that will contain the new states accessible from the current state and the iterating symbol.
            bitset<12345> new_states;

            // For each state in the current set of states.
            for (unsigned long long i = 0; i < processing_states.size(); ++i)
            {
                // Inserts all the states accessible with the given symbol.
                if (processing_states[i] && nfa->transitions.find(make_pair(i, symbol)) != nfa->transitions.end())
                {
                    bitset<12345> accessible_states = nfa->transitions[make_pair(i, symbol)];
                    for (unsigned long long j = 0; j < accessible_states.size(); ++j)
                    {
                        if (accessible_states[j])
                            new_states[j] = 1;
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
        for (unsigned long long i = 0; i < queued_state.size(); ++i)
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
    filepath += ".txt";
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
        file << translations[transition.first.first] << " " << translations[transition.second] << " "
             << transition.first.second << "\n";
    }

    file.close();

    // Return that the file was written successfully.
    return true;
}

//Returns a vector containing each node
const vector<unsigned long long> DFA::get_nodes_vector()
{
    vector<unsigned long long> v(state_amount);
    iota(begin(v), end(v), 0);
    return v;
}

//Returns the start and end node of every edge as a vector of pairs
const vector<pair<unsigned long long, unsigned long long>> DFA::get_edges_vector()
{
    vector<pair<unsigned long long, unsigned long long>> v;
    //For each transition, get the initial and final nodes
    for (const auto &transition : transitions)
    {
        v.push_back(make_pair(translations[transition.first.first], translations[transition.second]));
    }
    return v;
}

unordered_set<unsigned long long> DFA::get_final_states()
{
    unordered_set<unsigned long long> final_set;
    //For each final set, push it in to our set
    for (const auto &final_state : final_states)
    {
        final_set.insert(this->translations[final_state]);
    }
    return final_set;
}

//This returns a vector of transitions, each transition symbol refers to an edge in the edge vector
vector<char> DFA::get_transitions()
{
    vector<char> v;
    //For each transition, push te symbol
    for (const auto &transition : transitions)
    {
        v.push_back(transition.first.second);
    }
    return v;
}

void DFA::graph(string output_file)
{
    //Set up the nodes, edges, and transition symbols
    const vector<unsigned long long> nodes = get_nodes_vector();
    const vector<pair<unsigned long long, unsigned long long>> edges_no_trans = get_edges_vector(); //The start and end states of each edge
    vector<char> transitions = get_transitions();                                                   //The transition symbol for each edge
    const unsigned long long n_edges = edges_no_trans.size();
    unordered_set<unsigned long long> final_states_set = get_final_states(); //For determining whether a state should be displayed with a double circle

    struct Vertex
    {
        unsigned long long id;
        string shape;
    };
    struct Edge
    {
        string weight;
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
        if (final_states_set.find(edges_no_trans[i].first) == final_states_set.end())
        {
            vertex1_circle_shape = "circle"; //If the state/vertex isn't found in the final state set
        }
        else
        {
            vertex1_circle_shape = "doublecircle";
        }

        if (final_states_set.find(edges_no_trans[i].second) == final_states_set.end())
        {
            vertex2_circle_shape = "circle";
        }
        else
        {
            vertex2_circle_shape = "doublecircle";
        }
        add_edge(add_vertex(Vertex{edges_no_trans[i].first, vertex1_circle_shape}, g), add_vertex(Vertex{edges_no_trans[i].second, vertex2_circle_shape}, g), Edge{"\"" + string(1, transitions[i]) + "\""}, g);
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

bool DFA::checkIfValid(const string &chain)
{
    bitset<12345> node = initial_state;

    for (const char &c : chain)
    {
        if (transitions.find({node, c}) == transitions.end())
        {
            return false;
        }
        else
        {
            node = transitions[{node, c}];
        }
    }

    return final_states.find(node) != final_states.end();
}