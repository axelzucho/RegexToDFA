//
// Created by axelzucho on 10/04/20.
//

#include "Lexer.h"

#include "Regex.h"

Lexer::Lexer(const string &path) {
    fstream file;

    file.open(path);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    string token, regex;
    while (file >> token && file >> regex) {
        Regex reg;
        reg.computeRegex(regex);
        reg.convert_to_NFA();
        DFA dfa(reg.nfa);
        tokenized.emplace_back(make_pair(dfa, token));
    }
}

vector<string> Lexer::evaluate(const string &toParse) {
    vector<string> results = {};
    for (auto &singleTokenized : tokenized) {
        if (singleTokenized.first.checkIfValid(toParse)) {
            results.push_back(singleTokenized.second);
        }
    }
    return results;
}

vector<vector<string>> Lexer::evaluateFile(const string &path) {
    fstream file;

    file.open(path);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    vector<vector<string>> results = {};
    string token;
    while (file >> token) {
        auto single_result = evaluate(token);
        if(single_result.empty()) {
            throw std::runtime_error("The string " + token + " is invalid");
        }
        results.push_back(single_result);
    }
    return results;
}

void Lexer::printOutputToFile(const vector<vector<string>> &result, const string& path) {
    ofstream file;
    file.open(path);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    for(const auto& singleWord : result){
        for(const auto& singleToken : singleWord){
           file << singleToken;
        }
        file << " ";
    }
}
