SOURCES = NFA.cpp DFA.cpp Regex.cpp
NO_OUTPUT = no_output.out
TEXT_OUTPUT = text_output.out
ALL_OUTPUT = all_output.out

all: $(ALL_OUTPUT) $(NO_OUTPUT) $(TEXT_OUTPUT)

$(ALL_OUTPUT): allOutput.cpp $(SOURCES)
	g++ -std=c++11 $^ -Wall --pedantic -o $@

$(NO_OUTPUT): withoutOutput.cpp $(SOURCES)
	g++ -std=c++11 $^ -Wall --pedantic -o $@

$(TEXT_OUTPUT): textOutput.cpp $(SOURCES)
	g++ -std=c++11 $^ -Wall --pedantic -o $@

clean:
	rm -f $(NO_OUTPUT) $(ALL_OUTPUT) $(TEXT_OUTPUT) *.o *.zip

.PHONY: clean
