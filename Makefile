KERNEL := build/simplicity.elf

CC = cc
CFLAGS = -Wall -Wextra -O2 -pipe

INTERNALLDFLAGS := \
	-fno-pic -fpie \
	-Wl,-static,-pie,--no-dynamic-linker,-ztext \
	-static-pie    \
	-nostdlib      \
	-Tsrc/linker.ld    \
	-z max-page-size=0x1000

INTERNALCFLAGS := \
	-I.           \
	-std=gnu11    \
	-ffreestanding \
	-fno-stack-protector \
	-fno-pic -fpie \
	-mno-80387    \
	-mno-mmx      \
	-mno-3dnow    \
	-mno-sse      \
	-mno-sse2     \
	-mno-red-zone

CFILES := $(shell find ./src -type f -name '*.c')
OBJ	   := $(CFILES:.c=.o)

.PHONY: all clean

all: $(KERNEL)

$(KERNEL): $(OBJ)
	$(CC) $(INTERNALLDFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

clean:
	/usr/bin/rm -rf $(KERNEL) $(OBJ)
