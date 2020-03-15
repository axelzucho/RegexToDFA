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
    // All operators are applied to NodeGroups and return a single NodeGroup.
    struct NodeGroup {
        int initial_node;
        int final_node;
    };

    // The Regex in the infix form.
    string infix_form;
    // The Regex in the postfix form, used to transform it to NFA.
    string postfix_form;
    // The next index on which to create the following state.
    int next_index;

    NodeGroup apply_operator(const char &op, const NodeGroup &first, const NodeGroup &second);

    NodeGroup apply_concat(const NodeGroup &first, const NodeGroup &second);

    NodeGroup apply_or(const NodeGroup &first, const NodeGroup &second);

    NodeGroup apply_star(const NodeGroup &node_group);

    friend class DFA;

public:
    explicit Regex(const string &filepath);

    NFA nfa;

    void convert_to_NFA();
};

#endif //NFA_TO_DFA_REGEX_H
