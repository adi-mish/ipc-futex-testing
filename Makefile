CC = clang
CFLAGS = -Wall -O3
LDFLAGS = -lrt

SRCS = writer.c reader.c shared.c
OBJS = $(SRCS:.c=.o)
EXES = writer reader

all: $(EXES)

writer: writer.o shared.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

reader: reader.o shared.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c shared.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXES) $(OBJS)
