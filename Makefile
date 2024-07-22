# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -march=native -O3
LDFLAGS = -lrt `pkg-config --libs glib-2.0`

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SRCS = $(wildcard $(SRCDIR)/*.c)

# Object files
OBJS_WRITER = $(OBJDIR)/writer.o $(OBJDIR)/shared.o
OBJS_READER = $(OBJDIR)/reader.o $(OBJDIR)/shared.o

# Executable files
EXES = $(BINDIR)/writer $(BINDIR)/reader

# Default target
all: $(EXES)

# Linking writer executable
$(BINDIR)/writer: $(OBJS_WRITER)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# Linking reader executable
$(BINDIR)/reader: $(OBJS_READER)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@

# Compiling source files into object files
$(OBJDIR)/writer.o: $(SRCDIR)/writer.c $(SRCDIR)/shared.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $< -o $@

$(OBJDIR)/reader.o: $(SRCDIR)/reader.c $(SRCDIR)/shared.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shared.o: $(SRCDIR)/shared.c $(SRCDIR)/shared.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/writer $(BINDIR)/reader

# Phony targets
.PHONY: all clean
