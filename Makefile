CXX      = g++
CXXFLAGS = -Wall -Werror -std=c++17 -g -O

BIN_DIR = bin
OBJ_DIR = obj

GEN_BIN   = $(BIN_DIR)/maze-gen
SOLVE_BIN = $(BIN_DIR)/maze-solve
VIEW_BIN  = $(BIN_DIR)/maze-view
FINAL_BIN = $(BIN_DIR)/maze-final

GEN_OBJS   = $(OBJ_DIR)/main-gen.o   $(OBJ_DIR)/maze.o
SOL_OBJS   = $(OBJ_DIR)/main-solve.o $(OBJ_DIR)/maze.o $(OBJ_DIR)/solver.o $(OBJ_DIR)/path.o
VIEW_OBJS  = $(OBJ_DIR)/main-view.o $(OBJ_DIR)/maze.o
FINAL_OBJS = $(OBJ_DIR)/main-final.o $(OBJ_DIR)/maze.o $(OBJ_DIR)/solver.o $(OBJ_DIR)/path.o

all: directories $(GEN_BIN) $(SOLVE_BIN) $(VIEW_BIN) $(FINAL_BIN)

directories:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)

$(GEN_BIN): directories $(GEN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(GEN_OBJS)

$(SOLVE_BIN): directories $(SOL_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(SOL_OBJS)

$(VIEW_BIN): directories $(VIEW_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(VIEW_OBJS) -lmcpp

$(FINAL_BIN): directories $(FINAL_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(FINAL_OBJS)

$(OBJ_DIR)/%.o: %.cpp | directories
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Example makefile test - you can add more tests if you like, or manually code them in TESTING.md
# Run maze-solve on the 3x3 test case and diff stdout and stderr against expected output.
# A passing implementation produces zero diff on both streams.
test: $(SOLVE_BIN)
	@$(SOLVE_BIN) < tests/test3x3.in > /tmp/test3x3.out 2> /tmp/test3x3.err; \
	echo "=== stdout diff ==="; \
	diff tests/test3x3.expected_out /tmp/test3x3.out && echo "OK" || echo "FAIL"; \
	echo "=== stderr diff ==="; \
	diff tests/test3x3.expected_err /tmp/test3x3.err && echo "OK" || echo "FAIL"

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

.PHONY: all clean directories test