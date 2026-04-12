CC=gcc
CCOPTS=-Wall -Wextra
LIBS=

SRCS=$(filter-out ppm.c, $(wildcard *.c))
TARGETS=$(SRCS:.c=)

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

%: %.c ppm.c
	$(CC) $(CCOPTS) -o $@ $< ppm.c $(LIBS)
