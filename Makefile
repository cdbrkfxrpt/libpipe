CC=g++
CXXSTD=-std=c++17
CFLAGS=-Wall -Wextra -Wpedantic -Werror -O0 -g

TARGETS=tests

all: $(TARGETS)

%:%.cc
	$(CC) $(CXXSTD) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf $(TARGETS)
