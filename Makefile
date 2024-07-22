# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -march=native -O3 `pkg-config --cflags glib-2.0`
LDFLAGS = -lrt `pkg-config --libs glib-2.0`

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SRCS = $(wildcard $(SRCDIR)/*.c)

# Object files
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Executable files
EXES = $(BINDIR)/writer $(BINDIR)/reader

# Default target
all: $(EXES)

# Linking writer executable
$(BINDIR)/writer: $(OBJDIR)/writer.o $(OBJDIR)/shared.o
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Linking reader executable
$(BINDIR)/reader: $(OBJDIR)/reader.o $(OBJDIR)/shared.o
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compiling source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/shared.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/writer $(BINDIR)/reader

# Phony targets
.PHONY: all clean
