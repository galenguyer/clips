CC=gcc

CFLAGS=-Wall -Wextra -pedantic -fstack-protector-all -pedantic -ledit -lm -std=c99
SANITY_FLAGS=-Wfloat-equal -Wshadow -Wpointer-arith

SOURCE=clips.c lval.c mpc/mpc.c

OUTPUT=clips

ALL: $(OUTPUT)

debug: CFLAGS += -g -O0 -fsanitize=address
debug: $(OUTPUT)

release: CFLAGS += -static -O3
release: $(OUTPUT)

$(OUTPUT): Makefile $(SOURCE)
	$(CC) $(CFLAGS) $(SANITY_FLAGS) $(SOURCE) -o $(OUTPUT)
