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
    return 0;
}
