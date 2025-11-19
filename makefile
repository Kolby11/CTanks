CC = gcc

# Detect OS
ifeq ($(OS),Windows_NT)
    CFLAGS  = -Wall -Wextra -g -I. -I./server -I./client -I./shared
    LDFLAGS = -lws2_32
    RM      = del /Q
    RMDIR   = rmdir /S /Q
    MKDIR   = if not exist
    BIN_EXT = .exe
    PATH_SEP = \\
else
    CFLAGS  = -Wall -Wextra -g -I. -I./server -I./client -I./shared
    LDFLAGS =
    RM      = rm -f
    RMDIR   = rm -rf
    MKDIR   = mkdir -p
    BIN_EXT =
    PATH_SEP = /
endif

BIN_DIR = bin

SERVER_DIR = server
CLIENT_DIR = client
SHARED_DIR = shared

APP_SOURCES = \
    $(wildcard $(SERVER_DIR)/*.c $(SERVER_DIR)/*/*.c) \
    $(wildcard $(CLIENT_DIR)/*.c $(CLIENT_DIR)/*/*.c) \
    $(wildcard $(SHARED_DIR)/*.c $(SHARED_DIR)/*/*.c)

APP_OBJECTS = $(APP_SOURCES:.c=.o)

APP_BIN = $(BIN_DIR)$(PATH_SEP)app$(BIN_EXT)

all: $(BIN_DIR) $(APP_BIN)

$(BIN_DIR):
ifeq ($(OS),Windows_NT)
	@$(MKDIR) $(BIN_DIR) $(BIN_DIR)
else
	@$(MKDIR) $(BIN_DIR)
endif

# App target
$(APP_BIN): $(APP_OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	@del /Q $(SERVER_DIR)\*.o 2>nul || exit 0
	@del /Q $(SERVER_DIR)\connection\*.o 2>nul || exit 0
	@del /Q $(CLIENT_DIR)\*.o 2>nul || exit 0
	@del /Q $(CLIENT_DIR)\connection\*.o 2>nul || exit 0
	@del /Q $(SHARED_DIR)\*.o 2>nul || exit 0
	@del /Q $(BIN_DIR)\*.exe 2>nul || exit 0
	@if exist $(BIN_DIR) rmdir $(BIN_DIR) 2>nul || exit 0
else
	@find . -name '*.o' -delete
	@$(RMDIR) $(BIN_DIR)
endif

run-app: $(APP_BIN)
	@$(APP_BIN)

app: $(BIN_DIR) $(APP_BIN)

.PHONY: all clean app run-app
