# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Directories
SERVER_DIR = server
CLIENT_DIR = client
BUILD_DIR = build

# Source files
SERVER_SOURCES = $(SERVER_DIR)/main.c $(SERVER_DIR)/connection/socket.c
CLIENT_SOURCES = $(CLIENT_DIR)/main.c $(CLIENT_DIR)/connection/socket.c

# Object files (in build directory)
SERVER_OBJECTS = $(patsubst $(SERVER_DIR)/%.c,$(BUILD_DIR)/server/%.o,$(SERVER_SOURCES))
CLIENT_OBJECTS = $(patsubst $(CLIENT_DIR)/%.c,$(BUILD_DIR)/client/%.o,$(CLIENT_SOURCES))

# Output executables (in build directory)
SERVER_BIN = $(BUILD_DIR)/server
CLIENT_BIN = $(BUILD_DIR)/client

# Default target
all: $(SERVER_BIN) $(CLIENT_BIN)

# Server target
$(SERVER_BIN): $(SERVER_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

# Client target
$(CLIENT_BIN): $(CLIENT_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

# Compile server .c files to .o files
$(BUILD_DIR)/server/%.o: $(SERVER_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile client .c files to .o files
$(BUILD_DIR)/client/%.o: $(CLIENT_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run targets
run-server: $(SERVER_BIN)
	$(SERVER_BIN)

run-client: $(CLIENT_BIN)
	$(CLIENT_BIN)

# Phony targets
.PHONY: all clean server client run-server run-client

# Individual targets
server: $(SERVER_BIN)
client: $(CLIENT_BIN)