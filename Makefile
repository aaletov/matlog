CXX = g++
CXXFLAGS = -I/usr/local/include -std=c++17
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

install-buddy:
	wget https://sourceforge.net/projects/buddy/files/latest/download
	tar -xzf download
	rm download

	cd ./buddy-2.4 && ./configure && make && sudo make install
	rm -rf buddy-2.4

