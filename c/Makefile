# Output info (dirs, binaries):

BIN_DIR = bin

SIMPLE_TCP_ECHO = simple_tcp_echo
SIMPLE_UDP_ECHO = simple_udp_echo

# Compiler info:

CC = gcc
CFLAGS = -Wall

all: $(SIMPLE_TCP_ECHO) $(SIMPLE_UDP_ECHO)

# TODO: Same command, cannot be simplified?
$(SIMPLE_TCP_ECHO): simple_tcp_echo.c options.h options.c $(BIN_DIR)
	$(CC) $(FLAGS) $< options.c -o $(BIN_DIR)/$@

$(SIMPLE_UDP_ECHO): simple_udp_echo.c options.h options.c $(BIN_DIR)
	$(CC) $(FLAGS) $< options.c -o $(BIN_DIR)/$@

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)
