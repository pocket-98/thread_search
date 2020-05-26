# Makefile for parallel
# usage: make [FLAGS=<flags>]

FLAGS           ?= -g

BIN             = bin
MAIN            = src/main.cpp
SOURCES         = src/sync_reader.cpp
HEADERS         = src/sync_reader.hpp
TARGET          = a.out

GXXOPTS         = -Wall -Wextra -pedantic -std=c++11
LDLIBS          = -lpthread
CXXOPTS         = $(GXXOPTS) $(CXXFLAGS) $(FLAGS)

OBJ             = $(patsubst %.cpp,$(BIN)/%.o,$(SOURCES))
MAIN_OBJ        = $(patsubst %.cpp,$(BIN)/%.o,$(MAIN))

.PHONY: all

all: $(TARGET)

# compile .cpp source files to objects (.o)
$(BIN)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXOPTS) -o $@ -c $<

# link executable
$(TARGET): $(OBJ) $(MAIN_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXOPTS) -o $@ $^ $(LDLIBS)
	@echo "binary compiled: '$(TARGET)'"

# remove objects and executable
clean:
	$(RM) $(TARGET) $(OBJ) $(MAIN_OBJ)
