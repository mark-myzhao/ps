PROJECT := ps

DEST_DIR := build
OBJ_DIR := $(DEST_DIR)/obj
INCLUDE_DIR := include
SRC_DIR := src

CXX := mpic++
LINK := mpic++
CFLAG := -I $(INCLUDE_DIR) -g -c

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

$(OBJ_DIR)/$(TEST_FILE).o: $(SRC_DIR)/$(TEST_FILE).cpp mkdir
	$(CXX) $< $(CFLAG) -o $@  

$(CXX_OBJS): $(CXX_SRCS) mkdir
	$(CXX) $(SRC_DIR)/$(PROJECT)/$(patsubst %.o,%.cpp,$(@F)) $(CFLAG) -o $@  

mkdir:
	@if [ ! -d $(DEST_DIR) ]; then \
	  mkdir $(DEST_DIR); \
		mkdir $(OBJ_DIR); \
	else \
		if [ ! -d $(OBJ_DIR) ]; then \
		  mkdir $(OBJ_DIR); \
		fi; \
	fi

clean: mkdir
	rm -rf ./$(DEST_DIR)
