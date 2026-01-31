DIR := .
INCLUDE := $(DIR)/INCLUDE
OBJS := $(DIR)/objects

CXX := g++
CXXFLAGS := -std=c++23 -I$(INCLUDE)
LDLIBS := -lz

all: server

server: $(OBJS)/main.o $(OBJS)/helpers.o $(OBJS)/get.o $(OBJS)/post.o
	$(CXX) $(OBJS)/*.o -o server $(LDLIBS)

$(OBJS)/main.o: $(DIR)/main.cpp $(INCLUDE)/defs.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS)/helpers.o: $(DIR)/helpers.cpp $(INCLUDE)/defs.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS)/get.o: $(DIR)/get.cpp $(INCLUDE)/defs.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS)/post.o: $(DIR)/post.cpp $(INCLUDE)/defs.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJS)/*.o server

.PHONY: all clean

