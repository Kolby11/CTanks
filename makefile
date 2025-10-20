# Compiler
CC = gcc

# Detect OS
ifeq ($(OS),Windows_NT)
    # Windows-specific settings
    CFLAGS = -Wall -Wextra -g -I./shared
    LDFLAGS = -lws2_32
    RM = del /Q
    RMDIR = rmdir /S /Q
    MKDIR = if not exist
    BIN_EXT = .exe
    PATH_SEP = \\
else
    # Linux/Unix settings
    CFLAGS = -Wall -Wextra -g -I./shared
    LDFLAGS = 
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
    BIN_EXT =
    PATH_SEP = /
endif

# Directories
SERVER_DIR = server
CLIENT_DIR = client
SHARED_DIR = shared
BIN_DIR = bin

# Source files
SERVER_SOURCES = $(SERVER_DIR)/main.c $(SERVER_DIR)/connection.c
CLIENT_SOURCES = $(CLIENT_DIR)/main.c $(CLIENT_DIR)/connection.c
SHARED_SOURCES = $(wildcard $(SHARED_DIR)/*.c)

# Object files
SERVER_OBJECTS = $(SERVER_SOURCES:.c=.o)
CLIENT_OBJECTS = $(CLIENT_SOURCES:.c=.o)
SHARED_OBJECTS = $(SHARED_SOURCES:.c=.o)

# Output executables
SERVER_BIN = $(BIN_DIR)$(PATH_SEP)server$(BIN_EXT)
CLIENT_BIN = $(BIN_DIR)$(PATH_SEP)client$(BIN_EXT)

# Default target
all: $(BIN_DIR) $(SERVER_BIN) $(CLIENT_BIN)

# Create bin directory
$(BIN_DIR):
ifeq ($(OS),Windows_NT)
	@$(MKDIR) $(BIN_DIR) $(BIN_DIR)
else
	@$(MKDIR) $(BIN_DIR)
endif

# Server target
$(SERVER_BIN): $(SERVER_OBJECTS) $(SHARED_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Client target
$(CLIENT_BIN): $(CLIENT_OBJECTS) $(SHARED_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
ifeq ($(OS),Windows_NT)
	@del /Q $(SERVER_DIR)\*.o 2>nul || exit 0
	@del /Q $(CLIENT_DIR)\*.o 2>nul || exit 0
	@del /Q $(SHARED_DIR)\*.o 2>nul || exit 0
	@del /Q $(BIN_DIR)\*.exe 2>nul || exit 0
	@if exist $(BIN_DIR) rmdir $(BIN_DIR) 2>nul || exit 0
else
	@$(RM) $(SERVER_DIR)/*.o $(CLIENT_DIR)/*.o $(SHARED_DIR)/*.o
	@$(RMDIR) $(BIN_DIR)
endif

# Run targets
run-server: $(SERVER_BIN)
	@$(SERVER_BIN)

run-client: $(CLIENT_BIN)
	@$(CLIENT_BIN)

# Individual targets
server: $(BIN_DIR) $(SERVER_BIN)

client: $(BIN_DIR) $(CLIENT_BIN)

# Phony targets
.PHONY: all clean server client run-server run-client