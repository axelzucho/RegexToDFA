//Install boost with sudo apt-get install libboost-dev

#ifndef GRAPH_H
#define GRAPH_H



#include <utility>
#include <algorithm>
#include <bitset>
#include <unordered_map>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "NFA.h"
#include "DFA.h"

using namespace boost;
using namespace std;

typedef std::pair<int, int> Edge;
typedef adjacency_list<vecS, vecS, bidirectionalS> Graph_Adj_List;

class Graph {
private:
  DFA *dfa;
  NFA *nfa;

  //Number of states contained by the NFA or DFA
  unsigned long long state_amount;
  unsigned long long transition_amount;
  unordered_map<pair<bitset<128>, char>, bitset<128>, pair_bitset_char_hash, pair_bitset_char_comparator> transitions;
public:
  explicit Graph(DFA &dfa);
  explicit Graph(NFA &nfa);

  void graph_DFA();
  void graph_NFA();
};

#endif //GRAPH_H
