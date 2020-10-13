#CXX = g++ -Wall -Ofast --std=c++14
CXX = g++ -Wall -O2 --std=c++14
LIBS = -lboost_program_options
TESTLIBS = -lgtest -lgtest_main -lpthread -lboost_program_options
all : testAll make_all

test : testAll
	./testAll

testAll.o : position.h

make_all.o : position.h make_all.cc

make_from_init.o : position.h make_from_init.cc

retro.o : position.h retro.cc position_set.h

show1.o : position.h show1.cc position_set.h

show_position.o : position.h show_position.cc position_set.h

count_positions.o : position.h count_positions.cc position_set.h

find_positions.o : position.h find_positions.cc position_set.h

testAll : testAll.o
	$(CXX) -o $@ $<

make_all : make_all.o
	$(CXX) -o $@ $<

make_from_init : make_from_init.o
	$(CXX) -o $@ $<

retro : retro.o
	$(CXX) -o $@ $< $(LIBS)

show1 : show1.o
	$(CXX) -o $@ $<

show_position : show_position.o
	$(CXX) -o $@ $<  $(LIBS)


count_positions : count_positions.o
	$(CXX) -o $@ $<  $(LIBS)

find_positions : find_positions.o
	$(CXX) -o $@ $< $(LIBS)
