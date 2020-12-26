CXX      := g++
CXXFLAGS := -pedantic-errors -Wall -Wextra -fPIC -std=c++17
LDFLAGS  := -L/usr/lib -lstdc++ -lm -lpthread -shared

BUILD    	:= ./build
OBJ_DIR  	:= $(BUILD)/objects
LIB_DIR  	:= $(BUILD)/lib
TARGET_LIB      := convlib.so
INCLUDE  	:= -I./include -I/usr/include/python3.9

HDRS     := $(wildcard ./include/*.h) 
SRC      := $(wildcard ./src/*.cpp)

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(LIB_DIR)/$(TARGET_LIB)

$(OBJ_DIR)/%.o: %.cpp $(HDRS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(LIB_DIR)/$(TARGET_LIB): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(LIB_DIR)/$(TARGET_LIB) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(LIB_DIR)/*
