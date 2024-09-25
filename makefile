# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRCS = project1.c child_proc.c

# Executable names
EXEC1 = project1
EXEC2 = child

# Default target
all: $(EXEC1) $(EXEC2)

# Rule to compile project1
$(EXEC1): project1.c project1.h
	$(CC) $(CFLAGS) -o $(EXEC1) project1.c

# Rule to compile child_proc
$(EXEC2): child_proc.c project1.h
	$(CC) $(CFLAGS) -o $(EXEC2) child_proc.c

# Clean up generated files
clean:
	rm -f $(EXEC1) $(EXEC2)

.PHONY: all clean
