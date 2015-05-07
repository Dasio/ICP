PROJECT = labyrinth2015
BIN_CLIENT = $(PROJECT)-cli
BIN_SERVER = $(PROJECT)-server

BIN_DIR = bin
OBJ_DIR = obj
SERVER_DIR = server
CLIENT_DIR = client
COMMON_DIR = common
SRC_DIR = src
GUI_DIR = gui
QMAKE_DIR = qmake

LINK = -lboost_system -lpthread

CXX = g++
CFLAGS = -Wall -Wextra -std=c++11 -g

COMMON_SRCS = $(wildcard $(SRC_DIR)/$(COMMON_DIR)/*.cpp)
COMMON_OBJS = $(patsubst $(SRC_DIR)/$(COMMON_DIR)/%.cpp, $(OBJ_DIR)/$(COMMON_DIR)/%.o, $(COMMON_SRCS))

SERVER_SRCS = $(wildcard $(SRC_DIR)/$(SERVER_DIR)/*.cpp)
SERVER_OBJS = $(patsubst $(SRC_DIR)/$(SERVER_DIR)/%.cpp, $(OBJ_DIR)/$(SERVER_DIR)/%.o, $(SERVER_SRCS))

CLIENT_SRCS = $(wildcard $(SRC_DIR)/$(CLIENT_DIR)/*.cpp)
CLIENT_OBJS = $(patsubst $(SRC_DIR)/$(CLIENT_DIR)/%.cpp, $(OBJ_DIR)/$(CLIENT_DIR)/%.o, $(CLIENT_SRCS))

build: server client gui

server: $(COMMON_OBJS) $(SERVER_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CFLAGS) $^ -o $(BIN_DIR)/$(BIN_SERVER) $(LINK)

client: $(COMMON_OBJS) $(CLIENT_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CFLAGS) $^ -o $(BIN_DIR)/$(BIN_CLIENT) $(LBOOST) $(LINK)
gui:
	@cd $(GUI_DIR) && qmake -o ../$(QMAKE_DIR)/Makefile
	$(MAKE) CXX=$(CXX) -C $(QMAKE_DIR)

$(OBJ_DIR)/$(SERVER_DIR)/%.o: $(SRC_DIR)/$(SERVER_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/$(SERVER_DIR)
	$(CXX) $(CFLAGS) $< -c -o $@

$(OBJ_DIR)/$(CLIENT_DIR)/%.o: $(SRC_DIR)/$(CLIENT_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/$(CLIENT_DIR)
	$(CXX) $(CFLAGS) $< -c -o $@

$(OBJ_DIR)/$(COMMON_DIR)/%.o: $(SRC_DIR)/$(COMMON_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/$(COMMON_DIR)
	$(CXX) $(CFLAGS) $< -c -o $@
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(QMAKE_DIR)
.PHONY: build server client gui clean