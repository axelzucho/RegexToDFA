#include <iostream>

#include "NFA.h"
#include "DFA.h"
#include "Regex.h"

using namespace std;
/*
int main() {
    string input_file;
    string output_file;

    cout << "Please input the filepath for the input" << endl;
    cin >> input_file;

    // Creates an NFA from the input file.
    NFA nfa(input_file);
    // If we didn't find the file, just return.
    if(!nfa.found_file()){
        cout << "Could not open input file" << endl;
        return 0;
    }

    cout << "Please input the filepath for the output" << endl;
    cin >> output_file;

    // Creates a DFA from the given NFA.
    DFA dfa(nfa);
    // Writes the data to the specified path, checking if it was successful.
    if(dfa.write_to_file(output_file)){
        cout << "Output written successfully!\n";
    } else {
        cout << "Output could not be written in the provided path\n";
    }

    return 0;
}
*/

int main(){
    string input_file;
    string output_file;

    cout << "Please input the filepath for the input" << endl;
    cin >> input_file;

    Regex regex(input_file);
    cout << "Completed!\n";
}