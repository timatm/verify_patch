CC = gcc
CFLAGS = -Wall -g
INCULUDE  = $(shell pkg-config --configs libtiff)
LIBS = $(shell pkg-config --libs libtiff-4)

# Source files
SRCS = tiff_decom.c page_combine.c main.c

# .o file
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = tiff_decom.h page_combine.h config.h main.h

TARGET = tiff-test

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(TARGET) $(OBJS) $(LIBS)

# Pattern rule to build object files from source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
