# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99

# Target executable
TARGET = test_tempHumidtree

# Source files
SRCS = test_tempHumidtree.c tempHumidtree.c iom361_r2.c float_rndm.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET).exe $(OBJS)

# Compile individual object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET).exe
