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
    if(argc != 3) {
        cout << "Expected two arguments, the first one being the path to the input file with the regex to parse, "
                "and the second one the output path (without extension)" << endl;
        cout << "Instead, got " << argc - 1 << " arguments." << endl;
        return 0;
    }

    string input_file = argv[1];
    string output_file = argv[2];

    auto start = high_resolution_clock::now();
    Regex regex(input_file);
    regex.convert_to_NFA();
    DFA dfa(regex.nfa);

    // Writes the data to the specified path, checking if it was successful.
    if (dfa.write_to_file(output_file))
    {
        cout << "Output written successfully!\n";
    }
    else
    {
        cout << "Output could not be written in the provided path\n";
    }
    auto stop = high_resolution_clock::now();

    auto time_span = duration_cast<duration<double>>(stop - start);
    cout << "Took " << time_span.count() << " seconds to convert from Regex to DFA and to write the text output.\n";

    return 0;

}
