//
// Created by axelzucho on 17/03/20.
//

#include <iostream>
#include <chrono>

#include "NFA.h"
#include "DFA.h"
#include "Regex.h"

using namespace std;
using namespace std::chrono;
int main(int argc, char* argv[])
{
    if(argc != 2) {
        cout << "Expected one argument: Path to the input file with the regex to parse.\n";
        cout << "Instead, got " << argc - 1 << " arguments." << endl;
        return 0;
    }

    string input_file = argv[1];
    auto start = high_resolution_clock::now();
    Regex regex(input_file);
    regex.convert_to_NFA();
    DFA dfa(regex.nfa);
    auto stop = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(stop - start);

    cout << "Took " << time_span.count() << " seconds to convert from Regex to DFA.\n";

    while(true) {
        string chain;
        cout << "Enter a string to evaluate. If you want to exit, enter 'exit'\n";
        cin >> chain;
        start = high_resolution_clock::now();
        if(chain == "exit") {
            break;
        } else {
            if(dfa.checkIfValid(chain)) {
                stop = high_resolution_clock::now();
                cout << "Valid chain\n";
            } else {
                stop = high_resolution_clock::now();
                cout << "Invalid chain\n";
            }
        }
        time_span = duration_cast<duration<double>>(stop - start);
        cout << "Took " << time_span.count() << " seconds to check the given string.\n";
    }

    return 0;
}
