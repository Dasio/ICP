PROJECT = labyrinth2015
BIN_CLIENT = $(PROJECT)
BIN_SERVER = $(PROJECT)-server

BIN_DIR = bin
OBJ_DIR = obj
SERVER_DIR = server
CLIENT_DIR = client
SRC_DIR = src


CXX = g++
CFLAGS = -Wall -Wextra -std=c++11 -g

SERVER_SRCS = $(wildcard $(SRC_DIR)/$(SERVER_DIR)/*.cpp)
SERVER_OBJS = $(patsubst $(SRC_DIR)/$(SERVER_DIR)/%.cpp, $(OBJ_DIR)/$(SERVER_DIR)/%.o, $(SERVER_SRCS))

CLIENT_SRCS = $(wildcard $(SRC_DIR)/$(CLIENT_DIR)/*.cpp)
CLIENT_OBJS = $(patsubst $(SRC_DIR)/$(CLIENT_DIR)/%.cpp, $(OBJ_DIR)/$(CLIENT_DIR)/%.o, $(CLIENT_SRCS))

build: server client

server: $(SERVER_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CFLAGS) $^ -o $(BIN_DIR)/$(BIN_SERVER)

client: $(CLIENT_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CFLAGS) $^ -o $(BIN_DIR)/$(BIN_CLIENT)
$(OBJ_DIR)/$(SERVER_DIR)/%.o: $(SRC_DIR)/$(SERVER_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/$(SERVER_DIR)
	$(CXX) $(CFLAGS) $< -c -o $@

$(OBJ_DIR)/$(CLIENT_DIR)/%.o: $(SRC_DIR)/$(CLIENT_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/$(CLIENT_DIR)
	$(CXX) $(CFLAGS) $< -c -o $@

clean:
	rm -rf bin obj
