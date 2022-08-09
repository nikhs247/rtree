CXXFLAGS := -g

all: build/main.o

build/rtree.o: src/rtree.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^

build/main.o: build/rtree.o src/main.cc
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm build/*