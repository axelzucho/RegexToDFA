#include <iostream>

#include "NFA.h"
#include "DFA.h"

using namespace std;

int main() {
    string input_file;
    string output_file;

    cout << "Please input the filepath for the input" << endl;
    cin >> input_file;

    NFA nfa(input_file);
    if(!nfa.found_file()){
        cout << "Could not open input file" << endl;
        return 0;
    }

    cout << "Please input the filepath for the output" << endl;
    cin >> output_file;

    DFA dfa(nfa);
    if(dfa.write_to_file(output_file)){
        cout << "Output written successfully!\n";
    } else {
        cout << "Output could not be written in the provided path\n";
    }

    return 0;
}