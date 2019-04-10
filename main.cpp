#include <iostream>
#include "NFA.h"
#include "DFA.h"

int main() {
    NFA nfa("../NFA_example.txt");
    DFA dfa(nfa);
    dfa.write_to_file("../DFA_solution.txt");
    std::cout << "Hello, World!" << std::endl;
    return 0;
}