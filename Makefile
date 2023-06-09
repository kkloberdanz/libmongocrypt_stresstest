CC=cc
STD=-std=c99
WARN=-Werror -Wall -Wextra -Wpedantic -Wfloat-equal -Wundef -Wshadow \
	-Wpointer-arith -Wcast-align -Wstrict-prototypes -Wmissing-prototypes \
	-Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual \
	-Wswitch-enum -Wunreachable-code -Wformat -Wformat-security -Wvla
OPT=-O3 -g
SAN=-fsanitize=thread
# SAN=-fsanitize=address
OMP=-fopenmp

override CFLAGS += $(OPT) $(WARN) $(OMP) $(SAN) $(STD)
override LDFLAGS += -lpthread -lmongocrypt

libmongocrypt_stresstest: main.c
	$(CC) -o libmongocrypt_stresstest main.c $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f libmongocrypt_stresstest
