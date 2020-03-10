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
    bool opened_file;

    fstream file;

    string prefix_form;

    string postfix_form;

    NFA nfa;

    int biggest_node;

    int lowest_node;

    int current_node;

    friend class DFA;
public:
    explicit Regex(const string& filepath);

};


#endif //NFA_TO_DFA_REGEX_H
