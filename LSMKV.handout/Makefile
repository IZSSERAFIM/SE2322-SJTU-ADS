LINK.o = $(LINK.cc)
CXXFLAGS = -std=c++2a -Wall -g

all: correctness persistence efficiency

correctness: kvstore.o correctness.o bloomFilter.o

persistence: kvstore.o persistence.o bloomFilter.o

efficiency: kvstore.o efficiency.o bloomFilter.o

clean:
	-rm -f correctness persistence efficiency *.o