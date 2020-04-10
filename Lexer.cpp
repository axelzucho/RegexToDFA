//
// Created by axelzucho on 10/04/20.
//

#include "Lexer.h"

#include <fstream>

#include "Regex.h"
#include "DFA.h"

Lexer::Lexer(const string &path) {
    fstream file;

    file.open(path);
    // Check if we could open the provided filepath.
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    string token, regex;
    while(file >> token && file >> regex) {
        Regex reg;
        reg.computeRegex(regex);
        reg.convert_to_NFA();
        DFA dfa(reg.nfa);
        tokenized.emplace_back(make_pair(dfa, token));
    }
}

vector<string> Lexer::evaluate(const string& toParse){
    vector<string> results = {};
    for(auto& singleTokenized : tokenized) {
        if(singleTokenized.first.checkIfValid(toParse)) {
            results.push_back(singleTokenized.second);
        }
    }
    return results;
}
