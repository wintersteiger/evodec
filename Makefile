all: evodec

CXXFLAGS=-std=c++11 -Wall -pedantic
CXXFLAGS+=-g

SRC=main.cpp devices.cpp decode.cpp encode.cpp interpret.cpp
OBJ=$(SRC:.cpp=.o)
DEP=$(SRC:.cpp=.d)

include $(SRC:.cpp=.d)

evodec: $(OBJ) $(DEP)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	rm -rf $(OBJ) $(DEP) evodec