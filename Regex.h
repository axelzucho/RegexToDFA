//
// Created by axelzucho on 10/03/20.
//

#ifndef NFA_TO_DFA_REGEX_H
#define NFA_TO_DFA_REGEX_H



#include <set>
#include <map>
#include <fstream>

#include "NFA.h"

using namespace std;

class Regex {
private:
    // Shows if the input file could be opened.
    struct NodeGroup {
        int initial_node;
        int final_node;
    };

    bool opened_file;

    fstream file;

    string prefix_form;

    string postfix_form;

    int next_index;

    friend class DFA;

    NodeGroup apply_operator(const char& op, const NodeGroup& first, const NodeGroup& second);
    NodeGroup apply_concat(const NodeGroup& first, const NodeGroup& second);
    NodeGroup apply_or(const NodeGroup& first, const NodeGroup& second);
    NodeGroup apply_star(const NodeGroup& node_group);


public:
    explicit Regex(const string& filepath);
    NFA nfa;
    void convert_to_NFA();

};


#endif //NFA_TO_DFA_REGEX_H
