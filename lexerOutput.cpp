#include <chrono>
#include <iostream>

#include "Lexer.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    if(argc != 2) {
        cout << "Expected one argument, the path to the input file\n";
        cout << "Instead, got " << argc - 1 << " arguments." << endl;
        return 0;
    }

    string input_file = argv[1];

    auto start = high_resolution_clock::now();
    Lexer lexer(input_file);
    auto stop = high_resolution_clock::now();

    auto time_span = duration_cast<duration<double>>(stop - start);
    cout << "Took " << time_span.count() << " seconds to tokenize all regexes" << endl;

    while(true) {
        string chain;
        cout << "Enter a string to evaluate. If you want to exit, enter 'exit'\n";
        cin >> chain;
        start = high_resolution_clock::now();
        if(chain == "exit") {
            break;
        } else {
            const vector<string> allTokens = lexer.evaluate(chain);
            stop = high_resolution_clock::now();
            time_span = duration_cast<duration<double>>(stop - start);
            cout << "Took " << time_span.count() << " seconds to tokenize the input\n";
            if(allTokens.empty()) {
                cout << "The provided input had no valid tokens\n";
            } else {
                cout << "The input has the following tokens:\n";
                for(const auto& tk:allTokens) {
                    cout << tk << "\n";
                }
            }
        }
    }

    return 0;
}

