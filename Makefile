PROJECT := ps

EXE := mpirun

DEST_DIR := build
OBJ_DIR := $(DEST_DIR)/obj
INCLUDE_DIR := include
SRC_DIR := src

CXX := mpic++
LINK := mpic++
CFLAG := -I $(INCLUDE_DIR) -c
BOOST_FLAGS := -lboost_thread -lboost_system

CXX_SRCS := $(shell find $(SRC_DIR)/$(PROJECT) -name "*.cpp")
CXX_OBJS := $(foreach file, $(CXX_SRCS), \
	$(OBJ_DIR)/$(patsubst %.cpp,%.o, $(notdir $(file))))

TEST_FILE := main

# rules
all: $(DEST_DIR)/$(TEST_FILE)

test: $(DEST_DIR)/$(TEST_FILE)
	$(EXE) -n 1 $(DEST_DIR)/$(TEST_FILE)

run: $(DEST_DIR)/$(TEST_FILE)
	$(EXE) -n 4 $(DEST_DIR)/$(TEST_FILE)

$(DEST_DIR)/$(TEST_FILE): $(CXX_OBJS) $(OBJ_DIR)/$(TEST_FILE).o
	$(LINK) $^ -o $(DEST_DIR)/$(TEST_FILE) $(BOOST_FLAGS)
	@echo "[Success] Build Successful"

# compile: $(CXX_OBJS) $(OBJ_DIR)/$(TEST_FILE).o
# 	$(LINK) $^ -o $(DEST_DIR)/$(TEST_FILE) $(BOOST_FLAGS)
# 	@echo "[Success] Build Successful"

$(OBJ_DIR)/$(TEST_FILE).o: $(SRC_DIR)/$(TEST_FILE).cpp mkdir
	$(CXX) $< $(CFLAG) -o $@ $(BOOST_FLAGS)

$(CXX_OBJS): $(CXX_SRCS) mkdir
	$(CXX) $(SRC_DIR)/$(PROJECT)/$(patsubst %.o,%.cpp,$(@F)) $(CFLAG) -o $@ $(BOOST_FLAGS)

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
