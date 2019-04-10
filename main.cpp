#include <iostream>
#include "NFA.h"
#include "DFA.h"

int main() {
    NFA nfa("../NFA_example.txt");
    DFA dfa(nfa);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}