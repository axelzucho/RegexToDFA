#include <iostream>

#include "NFA.h"
#include "DFA.h"
#include "Regex.h"

using namespace std;

int main()
{
    string input_file;
    string output_file;

    cout << "Please input the filepath for the input" << endl;
    cin >> input_file;

    Regex regex(input_file);
    regex.convert_to_NFA();
    DFA dfa(regex.nfa);

    cout << "Please input the filepath for the output (without the file extension)" << endl;
    cin >> output_file;

    // Writes the data to the specified path, checking if it was successful.
    if (dfa.write_to_file(output_file))
    {
        cout << "Output written successfully!\n";
    }
    else
    {
        cout << "Output could not be written in the provided path\n";
    }

    dfa.graph(output_file);

    return 0;
}
