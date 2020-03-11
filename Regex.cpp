//
// Created by axelzucho on 10/03/20.
//

#include <fstream>
#include <iostream>
#include <stack>
#include "Regex.h"


string insertConcatOp(const string &s) {
    string result;

    for (int i = 0; i < s.length() - 1; i++) {
        const char &current = s[i];
        result += current;

        if (current == '(' || current == '|') continue;

        const char &next = s[i + 1];
        if (next != '*' && next != '|' && next != ')') {
            result += '.';
        }
    }

    result += s.back();
    return result;
}

string convertToPostfix(const string &s, const map<char, short int> &precedence) {
    string output;
    stack<char> ops;
    for (const char &c:s) {
        if (c == '.' || c == '|' || c == '*') {
            while (!ops.empty() && precedence.at(ops.top()) >= precedence.at(c)) {
                output += ops.top();
                ops.pop();
            }
            ops.push(c);
        } else if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output += ops.top();
                ops.pop();
            }
            // Getting rid of the '('
            ops.pop();
        } else {
            output += c;
        }
    }

    while (!ops.empty()) {
        output += ops.top();
        ops.pop();
    }

    return output;
}

Regex::Regex(const string &filepath) {
    this->next_index = 0;

    file.open(filepath);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        opened_file = false;
        return;
    }
    // We could open the file.
    opened_file = true;

    file >> prefix_form;
    this->prefix_form = insertConcatOp(this->prefix_form);

    map<char, short int> precedence = {
            {'(', 0},
            {'.', 1},
            {'|', 2},
            {'*', 3}};

    this->postfix_form = convertToPostfix(this->prefix_form, precedence);
    cout << "Prefix: " << this->prefix_form << "\n";
    cout << "Postfix: " << this->postfix_form << "\n";
}

bool isOperator(const char &op) {
    return (op == '.' || op == '*' || op == '|');
}

void Regex::convert_to_NFA() {
    stack<NodeGroup> node_groups;
    for (const char &c:this->postfix_form) {
        if (isOperator(c)) {
            if(c != '*') {
                NodeGroup second_operand = node_groups.top();
                node_groups.pop();
                NodeGroup first_operand = node_groups.top();
                node_groups.pop();
                node_groups.push(apply_operator(c, first_operand, second_operand));
            } else {
                NodeGroup to_star = node_groups.top();
                node_groups.pop();
                node_groups.push(apply_star(to_star));
            }
        } else {
            NodeGroup adding_group = {this->next_index++, this->next_index++};
            nfa.add_edge(adding_group.initial_node, adding_group.final_node, c);
            node_groups.push(adding_group);
        }
    }
    this->nfa.initial_node = {node_groups.top().initial_node};
    this->nfa.final_states = {node_groups.top().final_node};
    this->nfa.state_amount = this->next_index;
    this->nfa.alphabet_size = 4;
}

Regex::NodeGroup Regex::apply_operator(const char &op, const Regex::NodeGroup &first, const Regex::NodeGroup &second) {
    if (op == '.') {
        return apply_concat(first, second);
    } else if (op == '|') {
        return apply_or(first, second);
    } else {
        throw EXIT_FAILURE;
    }
}

Regex::NodeGroup Regex::apply_concat(const Regex::NodeGroup &first, const Regex::NodeGroup &second) {
    // TODO(axelzucho): Change 'f' to reflect new epsilon in all of these transitions.
    nfa.add_edge(first.final_node, second.initial_node, 'f');
    return NodeGroup{first.initial_node, second.final_node};
}

Regex::NodeGroup Regex::apply_or(const Regex::NodeGroup &first, const Regex::NodeGroup &second) {
    int first_node = this->next_index++;
    int last_node = this->next_index++;

    nfa.add_edge(first_node, first.initial_node, 'f');
    nfa.add_edge(first_node, second.initial_node, 'f');

    nfa.add_edge(first.final_node, last_node, 'f');
    nfa.add_edge(second.final_node, last_node, 'f');

    return NodeGroup{first_node, last_node};
}

Regex::NodeGroup Regex::apply_star(const Regex::NodeGroup &node_group) {
    int first_node = this->next_index++;
    int last_node = this->next_index++;

    nfa.add_edge(first_node, node_group.initial_node, 'f');
    nfa.add_edge(first_node, last_node, 'f');

    nfa.add_edge(node_group.final_node, node_group.initial_node, 'f');
    nfa.add_edge(node_group.final_node, last_node, 'f');

    return NodeGroup{first_node, last_node};
}


