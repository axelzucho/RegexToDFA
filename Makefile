SOURCES = main.cpp NFA.cpp DFA.cpp Regex.cpp
MAIN = automata_conversion.out

$(MAIN): $(SOURCES)
	g++ -std=c++11 $^ -Wall --pedantic -o $@

clean:
	rm -f $(MAIN) *.o *.zip

.PHONY: clean