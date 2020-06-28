CXX = g++ -Wall -Ofast -DNDEBUG --std=c++14
all : testAll make_all

test : testAll
	./testAll

testAll.o : position.h

make_all.o : position.h make_all.cc

make_from_init.o : position.h make_from_inti.cc

retro.o : position.h retro.cc position_set.h

show1.o : position.h show1.cc position_set.h

show_position.o : position.h show1.cc position_set.h

testAll : testAll.o
	$(CXX) -o $@ $<

make_all : make_all.o
	$(CXX) -o $@ $<

make_from_init : make_from_init.o
	$(CXX) -o $@ $<

retro : retro.o
	$(CXX) -o $@ $<

show1 : show1.o
	$(CXX) -o $@ $<

show_position : show_position.o
	$(CXX) -o $@ $<
