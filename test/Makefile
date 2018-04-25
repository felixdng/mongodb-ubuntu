CXX = g++
CXXFLAGS = --std=c++11
CXXLDFLAGS = -L /usr/local/mongo-cxx-driver-r3.1.2/lib -lmongocxx -lbsoncxx

SRCS = ${wildcard *.cpp}
OBJS = ${patsubst %.cpp,%,$(SRCS)}

%:%.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(CXXLDFLAGS)

#g++ --std=c++11 test.cpp -o test -L /usr/local/mongo-cxx-driver-r3.1.2/lib -lmongocxx -lbsoncxx

all: $(OBJS)

clean:
	rm -rf $(OBJS)
