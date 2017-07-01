PROJECT := ps

DEST_DIR := build
OBJ_DIR := $(DEST_DIR)/obj
INCLUDE_DIR := include
SRC_DIR := src

CXX := mpic++
LINK := mpic++
CFLAG := -I $(INCLUDE_DIR) -c

CXX_SRCS := $(shell find $(SRC_DIR)/$(PROJECT) -name "*.cpp")
CXX_OBJS := $(foreach file, $(CXX_SRCS), \
	$(OBJ_DIR)/$(patsubst %.cpp,%.o, $(notdir $(file))))

TEST_FILE := main

# rules
all: compile

run: compile
	$(DEST_DIR)/$(TEST_FILE)

compile: $(CXX_OBJS) $(OBJ_DIR)/$(TEST_FILE).o
	$(LINK) $^ -o $(DEST_DIR)/$(TEST_FILE)

$(OBJ_DIR)/$(TEST_FILE).o: $(SRC_DIR)/$(TEST_FILE).cpp
	$(CXX) $< $(CFLAG) -o $@  

$(CXX_OBJS): $(CXX_SRCS)
	$(CXX) $(SRC_DIR)/$(PROJECT)/$(patsubst %.o,%.cpp,$(@F)) $(CFLAG) -o $@  

clean:
	rm $(DEST_DIR)/main
	rm $(OBJ_DIR)/*
