DIR := ./src
INCLUDE := $(DIR)/INCLUDE
OBJS := $(DIR)/objects

CXX := g++
CXXFLAGS := -std=c++23 -I$(INCLUDE)
LDLIBS := -lz

all: server

server: objects $(OBJS)/main.o $(OBJS)/helpers.o $(OBJS)/get.o $(OBJS)/post.o
	$(CXX) $(OBJS)/*.o -o server $(LDLIBS)

$(OBJS)/main.o: $(DIR)/main.cpp $(INCLUDE)/defs.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS)/helpers.o: $(DIR)/helpers.cpp $(INCLUDE)/defs.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS)/get.o: $(DIR)/get.cpp $(INCLUDE)/defs.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS)/post.o: $(DIR)/post.cpp $(INCLUDE)/defs.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(INCLUDE)/defs.hpp:

objects:
	if [ -d "$(OBJS)" ]; then \
		rm -f "$(OBJS)"/*; \
	else \
		mkdir -p "$(OBJS)"; \
	fi

clean:
	rm -f $(OBJS)/*.o server

.PHONY: all clean

