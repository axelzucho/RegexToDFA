//
// Created by axelzucho on 10/04/20.
//

#ifndef NFA_TO_DFA_LEXER_H
#define NFA_TO_DFA_LEXER_H

#include <vector>

#include "DFA.h"

using namespace std;

class Lexer {
private:
    vector<pair<DFA, string>> tokenized;

public:
    explicit Lexer(const string& path);
    vector<string> evaluate(const string& toParse);
};


#endif //NFA_TO_DFA_LEXER_H
