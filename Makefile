CC=g++
CXXSTD=-std=c++17
CFLAGS=-Wall -Wextra -Wpedantic -Werror -O0 -g # -Wfatal-errors

TARGETS=pipe # fultz

all: $(TARGETS)

%:%.cc
	$(CC) $(CXXSTD) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf $(TARGETS)
