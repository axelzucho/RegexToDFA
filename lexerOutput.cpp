#include <chrono>
#include <iostream>

#include "Lexer.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    if(argc != 4) {
        cout << "Expected three arguments, the path to the regex input file, the path to the file to parse" <<
        " and the output file with the tokens";
        cout << "Instead, got " << argc - 1 << " arguments." << endl;
        return 0;
    }

    string input_file_lexer = argv[1];
    string input_file_parse = argv[2];
    string output_file = argv[3];

    auto start = high_resolution_clock::now();
    Lexer lexer(input_file_lexer);
    auto stop = high_resolution_clock::now();

    auto time_span = duration_cast<duration<double>>(stop - start);
    cout << "Took " << time_span.count() << " seconds to tokenize all regexes" << endl;

    auto result = lexer.evaluateFile(input_file_parse);

    lexer.printOutputToFile(result, output_file);

    return 0;
}

