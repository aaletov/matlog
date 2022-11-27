CXX = g++
CXXFLAGS = -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lbdd
objects = build/main.o build/conditions.o

all: build/main

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/main: $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o build/main $(LDFLAGS)

run:
	./build/main

clean:
	rm -rf build/*

