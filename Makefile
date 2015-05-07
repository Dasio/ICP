BIN_GAME = labyrinth2015-cli

BIN_DIR = bin
OBJ_DIR = obj
GAME_DIR = game
SRC_DIR = src
GUI_DIR = gui
QMAKE_DIR = qmake

CXX = g++
CFLAGS = -Wall -Wextra -std=c++11 -g

GAME_SRCS = $(wildcard $(SRC_DIR)/$(GAME_DIR)/*.cpp)
GAME_OBJS = $(patsubst $(SRC_DIR)/$(GAME_DIR)/%.cpp, $(OBJ_DIR)/$(GAME_DIR)/%.o, $(GAME_SRCS))

build: cli gui

cli: $(GAME_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CFLAGS) $^ -o $(BIN_DIR)/$(BIN_GAME)
gui:
	@cd $(GUI_DIR) && qmake -o ../$(QMAKE_DIR)/Makefile
	$(MAKE) CXX=$(CXX) -C $(QMAKE_DIR)

$(OBJ_DIR)/$(GAME_DIR)/%.o: $(SRC_DIR)/$(GAME_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/$(GAME_DIR)
	$(CXX) $(CFLAGS) $< -c -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(QMAKE_DIR)
.PHONY: build gui cli clean
